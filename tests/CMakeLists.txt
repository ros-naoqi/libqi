add_definitions(-DQI_IS_TEST)

include_directories(".")

add_library(
  qitestutils STATIC
  "qi/testutils/testutils.hpp"
  "qi/testutils/testutils.cpp"
  "qi/testutils/mockutils.hpp"
)

find_package(GTest REQUIRED)
target_link_libraries(qitestutils qi GTest::GTest)

add_subdirectory("ka")
# add_subdirectory("qi")
# add_subdirectory("type")
# add_subdirectory("messaging")
# add_subdirectory("perf")
