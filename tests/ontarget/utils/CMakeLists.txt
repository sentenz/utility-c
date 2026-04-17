# SPDX-License-Identifier: Apache-2.0

# ── Unit Tests ────────────────────────────────────────────────────────────────
#
# Registers the pure-computation utility tests for the on-target test binary.
# Only bit and numeric-utility tests are included here because they have no
# POSIX or OS dependencies and are well-suited for bare-metal execution.

meta_gtest(
    ENABLE ${META_BUILD_TESTING_ONTARGET}
    TARGET ${PROJECT_NAME}-ontarget-test
    SOURCES
        bit_test.cc
        util_test.cc
    LINK
        ${PROJECT_NAME}::utils
)
