# SPDX-License-Identifier: Apache-2.0

# ── On-Target Test Configuration ─────────────────────────────────────────────
#
# Each on-target test binary is cross-compiled for the ARM target and flashed
# via SEGGER J-Run.  meta_gtest() creates the executable (without CTest
# registration) and meta_jrun() registers it as a single CTest entry that
# invokes JRun to flash and run the binary on the connected J-Link target.

include(meta_gtest)
meta_gtest(
    ENABLE   ${META_BUILD_TESTING_ONTARGET}
    WITH_MAIN
    TARGET   ${PROJECT_NAME}-ontarget-test
)

include(meta_jrun)
meta_jrun(
    ENABLE    ${META_BUILD_TESTING_ONTARGET}
    TARGET    ${PROJECT_NAME}-ontarget-test
    DEVICE    ${META_JRUN_DEVICE}
    INTERFACE ${META_JRUN_INTERFACE}
    SPEED     ${META_JRUN_SPEED}
)

# ── Subdirectories ────────────────────────────────────────────────────────────

add_subdirectory(utils)
