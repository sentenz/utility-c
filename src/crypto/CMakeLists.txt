# Build library
if(UTILITYC_BUILD_LIB)
  # Add sources to target
  target_sources(${PROJECT_NAME} PRIVATE argon2.c)
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
    SOURCES argon2_test.cc
    LINK ${PROJECT_NAME})
endif() # UTILITYC_BUILD_TEST
