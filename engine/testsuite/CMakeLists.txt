# The entire C++ test suite uses GoogleTest for its testing framework.
INCLUDE(GoogleTest)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/engine/testsuite/gtest")

SET ( SOURCES
  main.cpp
  )
# Each of these subdirectories modifies the SOURCES variable,
# adding more source files
ADD_SUBDIRECTORY("gtest")
ADD_SUBDIRECTORY("compile-time")

ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("enumerate")
ADD_SUBDIRECTORY("link")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("misc")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surface")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")

ADD_EXECUTABLE(regina-testsuite ${SOURCES})
TARGET_LINK_LIBRARIES(regina-testsuite ${ENGINE_LIBRARY})

IF (NOT WIN32)
  # Split the many individual tests from regina-testsuite into many individual
  # CMake tests.  This means we can parallelise the test suite by running
  # `ctest -j...` or `make test ARGS=-j...`.
  GTEST_DISCOVER_TESTS(regina-testsuite)
ELSE (NOT WIN32)
  # On Windows we need to add the engine DLL location to the runtime path.
  # Unfortunately GTEST_DISCOVER_TESTS() does not support target properties
  # for the discovery run (it only passes them through when running the actual
  # tests).  This means test discovery will fail under windows (since the
  # engine DLL is not on the runtime path).  Fall back to the old method of
  # running the engine tests in serial, as a single CMake test.
  # See: https://gitlab.kitware.com/cmake/cmake/-/work_items/21453
  ADD_TEST(EngineTests.all regina-testsuite)

  # The string replacement is to prevent cmake treating $ENV{PATH} as a list.
  string(REPLACE ";" "\\;" ESCAPED_PATH "$ENV{PATH}")
  SET_PROPERTY(TEST EngineTests.all PROPERTY ENVIRONMENT
    "PATH=${ESCAPED_PATH}\;${PROJECT_BINARY_DIR}/engine")
ENDIF (NOT WIN32)

INSTALL(TARGETS regina-testsuite DESTINATION "${PKGLIBEXECDIR}"
  COMPONENT Runtime)

