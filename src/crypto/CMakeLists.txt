# Build library
if(UTILITYC_BUILD_LIB)
  # Add sources to target
  target_sources(${PROJECT_NAME} PRIVATE argon2.c)
endif() # UTILITYC_BUILD_LIB

# Create executable UnitTest
if(UTILITYC_BUILD_TEST)
  target_sources(${PROJECT_NAME}-test PRIVATE argon2_test.cc)
endif() # UTILITYC_BUILD_TEST
