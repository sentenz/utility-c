# SPDX-License-Identifier: Apache-2.0

ifneq (,$(wildcard .env))
	include .env
	export
endif

# Define Variables

PIP_VENV := .venv/bin

SHELL := bash
.SHELLFLAGS := -euo pipefail -c
.ONESHELL:

# Define Targets

default: help

# NOTE Targets MUST have a leading comment line starting with `##` to be included in the list. See the targets below for examples.
help:
	@awk 'BEGIN {printf "Tasks\n\tA collection of tasks used in the current project.\n\n"}'
	@awk 'BEGIN {printf "Usage\n\tmake $(shell tput -Txterm setaf 6)<task>$(shell tput -Txterm sgr0)\n\n"}' $(MAKEFILE_LIST)
	@awk '/^##/{c=substr($$0,3);next}c&&/^[[:alpha:]][[:alnum:]_-]+:/{print "$(shell tput -Txterm setaf 6)\t" substr($$1,1,index($$1,":")) "$(shell tput -Txterm sgr0)",c}1{c=0}' $(MAKEFILE_LIST) | column -s: -t
.PHONY: help

# ── Setup & Teardown ─────────────────────────────────────────────────────────────────────────────

## Initialize a software development workspace with requisites
bootstrap:
	@cd ./scripts/ && bash ./bootstrap.sh
.PHONY: bootstrap

## Install and configure all dependencies essential for development
setup:
	@cd ./scripts/ && bash ./setup.sh
.PHONY: setup

## Remove development artifacts and restore the host to its pre-setup state
teardown:
	@cd ./scripts/ && bash ./teardown.sh
.PHONY: teardown

# ── Build System ─────────────────────────────────────────────────────────────────────────────────

LOGS_PATH_TEST := logs/test

## Generate a CMake project configured for GCC-ARM in Debug mode
cmake-gcc-debug-configure:
	cmake --preset debug
.PHONY: cmake-gcc-debug-configure

## Compile the GCC-ARM project in Debug configuration
cmake-gcc-debug-build: cmake-gcc-debug-configure
	cmake --build --preset debug
.PHONY: cmake-gcc-debug-build

## Clean the GCC-ARM project in Debug configuration
cmake-gcc-debug-clean:
	cmake --build --preset debug --target clean
.PHONY: cmake-gcc-debug-clean

## Generate a CMake project configured for GCC-ARM in Release mode
cmake-gcc-release-configure:
	cmake --preset release
.PHONY: cmake-gcc-release-configure

## Compile the GCC-ARM project in Release configuration
cmake-gcc-release-build: cmake-gcc-release-configure
	cmake --build --preset release
.PHONY: cmake-gcc-release-build

## Clean the GCC-ARM project in Release configuration
cmake-gcc-release-clean:
	cmake --build --preset release --target clean
.PHONY: cmake-gcc-release-clean

## Generate a CMake project configured for unit tests
cmake-gcc-test-unit-configure:
	cmake --preset test
.PHONY: cmake-gcc-test-unit-configure

## Compile the unit test
cmake-gcc-test-unit-build: cmake-gcc-test-unit-configure
	cmake --build --preset test
.PHONY: cmake-gcc-test-unit-build

## Run the unit tests
cmake-gcc-test-unit-run: cmake-gcc-test-unit-build
	@mkdir -p "$(CURDIR)/${LOGS_PATH_TEST}"
	ctest --preset test --output-junit "$(CURDIR)/${LOGS_PATH_TEST}/junit.xml"
.PHONY: cmake-gcc-test-unit-run

## Clean the unit test build artifacts
cmake-gcc-test-unit-clean:
	cmake --build --preset test --target clean
.PHONY: cmake-gcc-test-unit-clean

## Run the GCC unit tests and generate code coverage report
cmake-gcc-test-unit-coverage:
	$(MAKE) cmake-gcc-test-unit-run || true
	$(MAKE) analysis-dynamic-coverage
.PHONY: cmake-gcc-test-unit-coverage

# ── Software Analysis ────────────────────────────────────────────────────────────────────────────

LOGS_PATH_COVERAGE := logs/coverage

## Generate code coverage report from dynamic analysis
analysis-dynamic-coverage:
	@mkdir -p "$(CURDIR)/${LOGS_PATH_COVERAGE}"
	gcovr --xml-pretty --print-summary --cobertura --output "$(CURDIR)/${LOGS_PATH_COVERAGE}/cobertura.xml" --html="$(CURDIR)/${LOGS_PATH_COVERAGE}/" --filter "src/" --exclude-unreachable-branches --gcov-ignore-parse-errors=suspicious_hits.warn_once_per_file
.PHONY: analysis-dynamic-coverage

# ── SAST Manager ─────────────────────────────────────────────────────────────────────────────────

SAST_IMAGE_TRIVY ?= aquasec/trivy:0.68.2@sha256:05d0126976bdedcd0782a0336f77832dbea1c81b9cc5e4b3a5ea5d2ec863aca7
SAST_IMAGE_COSIGN ?= cgr.dev/chainguard/cosign:3.0.0@sha256:b6bc266358e9368be1b3d01fca889b78d5ad5a47832986e14640c34a237ef638

## Scan Infrastructure-as-Code (IaC) files for misconfigurations using Trivy and generate a report
sast-trivy-misconfig:
	@mkdir -p logs/sast

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" config --output logs/sast/trivy-misconfig.json /workspace 2>&1
.PHONY: sast-trivy-misconfig

## Scan local filesystem for vulnerabilities and misconfigurations using Trivy
sast-trivy-fs:
	@mkdir -p logs/sast

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" filesystem --output logs/sast/trivy-filesystem.json /workspace 2>&1
.PHONY: sast-trivy-fs

# Usage: make sast-trivy-image <image_name>
#
## Scan a container image for vulnerabilities using Trivy
sast-trivy-image:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-image <image_name>"; \
		exit 1; \
	fi

	docker run --rm -v /var/run/docker.sock:/var/run/docker.sock -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" image --output logs/sast/trivy-image.json "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-image

# Usage: make sast-trivy-image-license <image_name>
#
## Scan a container image for license compliance using Trivy
sast-trivy-image-license:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-image-license <image_name>"; \
		exit 1; \
	fi

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" image --scanners license --format table --output logs/sast/trivy-image-license.txt "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-image-license

# Usage: make sast-trivy-repository <repo_url>
#
## Scan a remote repository for vulnerabilities using Trivy
sast-trivy-repository:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-repository <repo_url>"; \
		exit 1; \
	fi

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" repository --output logs/sast/trivy-repository.json "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-repository

# Usage: make sast-trivy-rootfs <path>
#
## Scan a rootfs e.g. `/` for vulnerabilities using Trivy
sast-trivy-rootfs:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-rootfs <path>"; \
		exit 1; \
	fi

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" rootfs --output logs/sast/trivy-rootfs.json "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-rootfs

# Usage: make sast-trivy-sbom-scan <sbom_path>
#
## Scan SBOM for vulnerabilities using Trivy
sast-trivy-sbom-scan:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-sbom-scan <sbom_path>"; \
		exit 1; \
	fi

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" sbom --output logs/sast/trivy-sbom.json "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-sbom-scan

# Usage: make sast-trivy-sbom-cyclonedx-image <image_name>
#
## Generate SBOM in CycloneDX format for a container image using Trivy
sast-trivy-sbom-cyclonedx-image:
	@mkdir -p logs/sbom

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-sbom-cyclonedx-image <image_name>"; \
		exit 1; \
	fi

	docker run --rm -v /var/run/docker.sock:/var/run/docker.sock -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" image --format cyclonedx --output logs/sbom/sbom-image.cdx.json "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-sbom-cyclonedx-image

# Usage: make sast-trivy-sbom-cyclonedx-fs <path>
#
## Generate SBOM in CycloneDX format for a file system using Trivy
sast-trivy-sbom-cyclonedx-fs:
	@mkdir -p logs/sbom

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-sbom-cyclonedx-fs <path>"; \
		exit 1; \
	fi

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" filesystem --format cyclonedx --output logs/sbom/sbom-fs.cdx.json "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-sbom-cyclonedx-fs

# Usage: make sast-trivy-sbom-license <sbom_path>
#
## Scan SBOM for license compliance using Trivy
sast-trivy-sbom-license:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-sbom-license <sbom_path>"; \
		exit 1; \
	fi

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" sbom --scanners license --format table --output logs/sast/trivy-sbom-license.txt "$(filter-out $@,$(MAKECMDGOALS))" 2>&1
.PHONY: sast-trivy-sbom-license

# Usage: make sast-trivy-sbom-attestation <intoto_sbom_path>
#
## Scan the verified SBOM attestation using Trivy
sast-trivy-sbom-attestation:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-trivy-sbom-attestation <intoto_sbom_path>"; \
		exit 1; \
	fi

	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_TRIVY)" sbom "$(filter-out $@,$(MAKECMDGOALS))"
.PHONY: sast-trivy-sbom-attestation

## Generate Cosign key pair
sast-cosign-generate-key-pair:
	docker run --rm -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_COSIGN)" generate-key-pair
.PHONY: sast-cosign-generate-key-pair

# Usage: make sast-cosign-attest <image_name>
#
## Attest an image with the generated SBOM using Cosign
sast-cosign-attest:
	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-cosign-attest <image_name>"; \
		exit 1; \
	fi
	@if [ ! -f cosign.key ]; then \
		echo "Error: cosign.key not found. Run 'make sast-cosign-generate-key-pair' first."; \
		exit 1; \
	fi
	@if [ ! -f logs/sbom/sbom.cdx.json ]; then \
		echo "Error: logs/sbom/sbom.cdx.json not found. Run 'make sast-trivy-sbom-cyclonedx <image_name>' first."; \
		exit 1; \
	fi

	docker run --rm -v "${HOME}/.docker/config.json:/root/.docker/config.json" -v "${PWD}:/workspace" -w /workspace -e COSIGN_PASSWORD "$(SAST_IMAGE_COSIGN)" attest --key cosign.key --type cyclonedx --predicate logs/sbom/sbom.cdx.json "$(filter-out $@,$(MAKECMDGOALS))"
.PHONY: sast-cosign-attest

# Usage: make sast-cosign-verify <image_name>
#
## Verify SBOM attestation for an image using Cosign
sast-cosign-verify:
	@mkdir -p logs/sast

	@if [ -z "$(filter-out $@,$(MAKECMDGOALS))" ]; then \
		echo "usage: make sast-cosign-verify <image_name>"; \
		exit 1; \
	fi
	@if [ ! -f cosign.pub ]; then \
		echo "Error: cosign.pub not found. Run 'make sast-cosign-generate-key-pair' first."; \
		exit 1; \
	fi

	docker run --rm -v "${HOME}/.docker/config.json:/root/.docker/config.json" -v "${PWD}:/workspace" -w /workspace "$(SAST_IMAGE_COSIGN)" verify-attestation --key cosign.pub --type cyclonedx "$(filter-out $@,$(MAKECMDGOALS))" > logs/sbom/sbom.cdx.intoto.jsonl 2> logs/sast/cosign-verify.log
.PHONY: sast-cosign-verify
