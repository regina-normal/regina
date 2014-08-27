# Give the test suite a way to locate the data files shipped with
# Regina's calculation engine.
SET_PROPERTY ( DIRECTORY
  PROPERTY COMPILE_DEFINITIONS
    TESTSUITE_DATA_HOME=\"${PROJECT_SOURCE_DIR}/engine\"
)

SET ( SOURCES
  exhaustive
  runalltests
  testparams
  )
# Each of these subdirectories modifies the SOURCES variable,
# adding more source files
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("generic")
ADD_SUBDIRECTORY("maths")
IF (NOT EXCLUDE_SNAPPEA)
ADD_SUBDIRECTORY("snappea")
ENDIF (NOT EXCLUDE_SNAPPEA)
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surfaces")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")

INCLUDE_DIRECTORIES(${ENGINE_INCLUDES})
INCLUDE_DIRECTORIES(${CPPUNIT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/engine)
ADD_EXECUTABLE(regtestsuite ${SOURCES})
TARGET_LINK_LIBRARIES(regtestsuite ${ENGINE_LIBRARY} ${CPPUNIT_LIBRARY}) # TODO

ADD_TEST(cpp-test regtestsuite)
