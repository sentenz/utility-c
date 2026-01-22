# ── Include Target ─────────────────────────────────────────────────────────────────────────────

# Provide an interface target for including headers
add_library(${PROJECT_NAME}-interface INTERFACE)
target_include_directories(${PROJECT_NAME}-interface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)
target_compile_definitions(${PROJECT_NAME}-interface INTERFACE
    S_HAVE_CONFIG_H
    S_POSIX
    HAVE_CONFIG_H
)

# Export a namespaced alias for subprojects and downstream consumers to link
add_library(${PROJECT_NAME}::interface ALIAS ${PROJECT_NAME}-interface)

# ── Unit Tests ───────────────────────────────────────────────────────────────────────────────────

include(meta_gtest)
meta_gtest(
    ENABLE ${META_BUILD_TESTING}
    WITH_CTEST
    WITH_MAIN
    TARGET ${PROJECT_NAME}-test
)

# ── Subdirectories ───────────────────────────────────────────────────────────────────────────────

add_subdirectory(authentication)
add_subdirectory(crypto)
add_subdirectory(utils)
