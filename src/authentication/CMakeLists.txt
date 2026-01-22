# ── Library Target ───────────────────────────────────────────────────────────────────────────────

add_library(${PROJECT_NAME}-authentication STATIC)

target_sources(
    ${PROJECT_NAME}-authentication
    PRIVATE
        password.c
)

target_link_libraries(${PROJECT_NAME}-authentication
    PUBLIC ${PROJECT_NAME}::interface
    PRIVATE ${PROJECT_NAME}::crypto
)

# Export a namespaced alias for subprojects and downstream consumers to link
add_library(${PROJECT_NAME}::authentication ALIAS ${PROJECT_NAME}-authentication)

# ── Unit Tests ───────────────────────────────────────────────────────────────────────────────────

meta_gtest(
    ENABLE ${META_BUILD_TESTING}
    TARGET ${PROJECT_NAME}-test
    SOURCES
        password_test.cc
    LINK
        ${PROJECT_NAME}::authentication
)
