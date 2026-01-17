# Build library
if(UTILITYC_BUILD_LIB)
  # Add sources to target
  target_sources(
    ${PROJECT_NAME}
    PRIVATE network.c
            regex.c
            util.c
            char.c
            bit.c
            uri.c
            fs.c
            xml.c)
endif() # UTILITYC_BUILD_LIB

# Build tests using meta_gtest
if(UTILITYC_BUILD_TEST)
  include(meta_gtest)
  
  meta_gtest(
    WITH_GMOCK
    WITH_MAIN
    WITH_CTEST
    TARGET ${PROJECT_NAME}-test
    ENABLE ${UTILITYC_BUILD_TEST}
    SOURCES network_test.cc
            regex_test.cc
            util_test.cc
            char_test.cc
            bit_test.cc
            uri_test.cc
            fs_test.cc
            xml_test.cc
    LINK ${PROJECT_NAME})
endif() # UTILITYC_BUILD_TEST

add_subdirectory(authentication)
add_subdirectory(crypto)
