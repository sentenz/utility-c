# Build library
if(UTILITYC_BUILD_LIB)
  # Add sources to target
  target_sources(${PROJECT_NAME} PRIVATE password.c)
endif() # UTILITYC_BUILD_LIB

# Create executable UnitTest
if(UTILITYC_BUILD_TEST)
  target_sources(${PROJECT_NAME}-test PRIVATE password_test.cc)
endif() # UTILITYC_BUILD_TEST
