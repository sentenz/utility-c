# Set the project name
project(mxml-3.1)

# Add a library with the above sources
add_library(
  ${PROJECT_NAME}
  include/mxml-attr.c
  include/mxml-entity.c
  include/mxml-file.c
  include/mxml-get.c
  include/mxml-index.c
  include/mxml-node.c
  include/mxml-private.c
  include/mxml-search.c
  include/mxml-set.c
  include/mxml-string.c)

add_library(external::libmxml ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC include)
