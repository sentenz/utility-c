# ── Library Target ───────────────────────────────────────────────────────────────────────────────

find_package(uriparser REQUIRED)

add_library(${PROJECT_NAME}-utils STATIC)

target_sources(
    ${PROJECT_NAME}-utils
    PRIVATE
        regex.c
        util.c
        char.c
        bit.c
        mutex.c
        uri.c
        fs.c
        xml.c
        network.c
)

target_link_libraries(${PROJECT_NAME}-utils
    PUBLIC ${PROJECT_NAME}::interface
    PUBLIC external::libmxml
    PRIVATE uriparser::uriparser
)

# Export a namespaced alias for subprojects and downstream consumers to link
add_library(${PROJECT_NAME}::utils ALIAS ${PROJECT_NAME}-utils)

# ── Unit Tests ───────────────────────────────────────────────────────────────────────────────────

meta_gtest(
    ENABLE ${META_BUILD_TESTING}
    TARGET ${PROJECT_NAME}-test
    SOURCES
        regex_test.cc
        util_test.cc
        char_test.cc
        network_test.cc
        bit_test.cc
        mutex_test.cc
        uri_test.cc
        fs_test.cc
        xml_test.cc
    LINK
        ${PROJECT_NAME}::utils
)
