# The entire C++ test suite uses GoogleTest for its testing framework.
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/engine/testsuite/gtest")

SET ( SOURCES
  main.cpp
  testexhaustive.cpp
  )
# Each of these subdirectories modifies the SOURCES variable,
# adding more source files
ADD_SUBDIRECTORY("gtest")
ADD_SUBDIRECTORY("compile-time")

ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("dim3")
ADD_SUBDIRECTORY("dim4")
ADD_SUBDIRECTORY("generic")
# ADD_SUBDIRECTORY("link")
ADD_SUBDIRECTORY("manifold")
# ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("misc")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surface")
ADD_SUBDIRECTORY("utilities")

ADD_EXECUTABLE(regina-testsuite ${SOURCES})
TARGET_LINK_LIBRARIES(regina-testsuite ${ENGINE_LIBRARY})

ADD_TEST(cpp-test regina-testsuite)
IF (WIN32)
  # On Windows we need to add the engine DLL location to the runtime path.
  # The string replacement is to prevent cmake treating $ENV{PATH} as a list.
  string(REPLACE ";" "\\;" ESCAPED_PATH "$ENV{PATH}")
  SET_PROPERTY(TEST cpp-test PROPERTY ENVIRONMENT
    "PATH=${ESCAPED_PATH}\;${PROJECT_BINARY_DIR}/engine")
ENDIF (WIN32)

INSTALL(TARGETS regina-testsuite DESTINATION "${PKGLIBEXECDIR}"
  COMPONENT Runtime)

