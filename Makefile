# See https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
help: ## Display help screen
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m<target>\033[0m\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  \033[36m%-30s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)
.PHONY: help

setup: ## Setup dependencies and tools
	cd tools/scripts && chmod +x setup.sh && ./setup.sh
.PHONY: setup

setup-devops: ## Setup dependencies and tools for the devops service
	cd tools/devops/scripts && chmod +x setup.sh && ./setup.sh
.PHONY: setup-devops

cleanup-devops: ## Cleanup dependencies and tools of the devops service
.PHONY: cleanup-devops

setup-integration: ## Setup dependencies and tools for the integration service
	cd scripts/pipeline && chmod +x setup_continuous_integration.sh && ./setup_continuous_integration.sh
.PHONY: setup-integration

setup-build: ## Setup dependencies and tools for the build service
	cd scripts/pipeline && chmod +x setup_continuous_build.sh && ./setup_continuous_build.sh
.PHONY: setup-build

run-linter-staged: ## Perform validation of local staged files
	cd tools/devops/cmd/app && chmod +x sast.sh && ./sast.sh -l staged
.PHONY: run-linter-staged

run-linter-diff: ## Perform validation of local modified files
	cd tools/devops/cmd/app && chmod +x sast.sh && ./sast.sh -l diff
.PHONY: run-linter-diff

run-linter-ci: ## Perform validation of modified files in continuous integration pipeline
	cd tools/devops/cmd/app && chmod +x sast.sh && ./sast.sh -l ci
.PHONY: run-linter-ci

run-linter-commit: ## Perform validation of the commit message
	commitlint --edit .git/COMMIT_EDITMSG
.PHONY: run-linter-commit

run-sanitizer-app: ## Perform validation of the application binary file
	cd tools/devops/cmd/app && chmod +x dast.sh && ./dast.sh -b cmd/bin/*-app
.PHONY: run-sanitizer-app

run-sanitizer-test: ## Perform validation of the test binary file
	cd tools/devops/cmd/app && chmod +x dast.sh && ./dast.sh -b cmd/bin/*-test
.PHONY: run-sanitizer-test

setup-testing: ## Setup dependencies and tools for the testing service
	cd scripts/pipeline && chmod +x setup_continuous_testing.sh && ./setup_continuous_testing.sh
.PHONY: setup-testing

setup-release: ## Setup dependencies and tools for the release service
	cd scripts/pipeline && chmod +x setup_continuous_release.sh && ./setup_continuous_release.sh
.PHONY: setup-release

run-release: ## Perform release service task
	npx semantic-release
.PHONY: run-release

setup-devcontainer: ## Setup dependencies and tools for the vscode devcontainer
	$(MAKE) setup
	$(MAKE) setup-devops
.PHONY: setup-devcontainer

setup-continuous-integration: ## Setup dependencies and tools for the continuous integration pipeline
	$(MAKE) setup-integration
  # Setup build environment to run cmake configure. This is needed to generate compile_command.json for clang-tidy
	$(MAKE) setup-build
	$(MAKE) all-configure
.PHONY: setup-continuous-integration

run-continuous-integration: ## Perform task in continuous integration pipeline
	$(MAKE) run-linter-ci
.PHONY: run-continuous-integration

setup-continuous-build: ## Setup dependencies and tools for the continuous build pipeline
	$(MAKE) setup-build
.PHONY: setup-continuous-build

run-continuous-build: ## Perform task in continuous build pipeline
	$(MAKE) all-configure
	$(MAKE) app-build
.PHONY: run-continuous-build

setup-continuous-testing: ## Setup dependencies and tools for the continuous testing pipeline
	$(MAKE) setup-testing
	$(MAKE) test-configure
	$(MAKE) test-build
	$(MAKE) test-coverage
.PHONY: setup-continuous-testing

run-continuous-testing: ## Perform task in continuous testing pipeline
	$(MAKE) run-sanitizer-test
.PHONY: run-continuous-testing

setup-continuous-release: ## Setup dependencies and tools for the continuous release pipeline
	$(MAKE) setup-release
.PHONY: setup-continuous-release

run-continuous-release: ## Perform task in continuous release pipeline
	$(MAKE) run-release
.PHONY: run-continuous-release

lib-configure: ## Configure library
	cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DUTILITYC_BUILD_TEST:BOOL=FALSE -DUTILITYC_BUILD_TEST_COVERAGE:BOOL=FALSE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc-9 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-9 -B build/cmake/build -G "Unix Makefiles"
.PHONY: lib-configure

lib-build: ## Build library
	cmake --build build/cmake/build --config Debug --target all --parallel --
.PHONY: lib-build

test-configure: ## Configure Unit tests
	cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DUTILITYC_BUILD_TEST:BOOL=TRUE -DUTILITYC_BUILD_TEST_COVERAGE:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc-9 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-9 -B build/cmake/build -G "Unix Makefiles"
.PHONY: test-configure

test-build: ## Build Unit tests
	cmake --build build/cmake/build --config Debug --target all --parallel --
.PHONY: test-build

test-coverage: ## Generate Unit tests code coverage
	cmake --build build/cmake/build --config Debug --target utility-c-coverage --parallel  --
.PHONY: test-coverage

test-run: ## Run Unit tests
	 GTEST_OUTPUT=xml:logs/test/result.xml GTEST_COLOR=1 ./cmd/bin/utility-c-test
.PHONY: test-run
