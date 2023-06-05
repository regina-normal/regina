SET ( SOURCES
  exhaustive.cpp
  runalltests.cpp
  testsuite.cpp
  )
# Each of these subdirectories modifies the SOURCES variable,
# adding more source files
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("dim3")
ADD_SUBDIRECTORY("dim4")
ADD_SUBDIRECTORY("link")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("surface")

INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})
INCLUDE_DIRECTORIES(${CPPUNIT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/engine)
ADD_EXECUTABLE(regtestsuite ${SOURCES})
TARGET_LINK_LIBRARIES(regtestsuite ${ENGINE_LIBRARY} ${CPPUNIT_LIBRARY}) # TODO

ADD_TEST(cpp-test regtestsuite)
IF (WIN32)
  # On Windows we need to add the engine DLL location to the runtime path.
  # The string replacement is to prevent cmake treating $ENV{PATH} as a list.
  string(REPLACE ";" "\\;" ESCAPED_PATH "$ENV{PATH}")
  SET_PROPERTY(TEST cpp-test PROPERTY ENVIRONMENT
    "PATH=${ESCAPED_PATH}\;${PROJECT_BINARY_DIR}/engine")
ENDIF (WIN32)

