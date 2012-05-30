# Engine

SET ( SOURCES globalarray modulemain )
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("file")
ADD_SUBDIRECTORY("foreign")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("packet")
ADD_SUBDIRECTORY("progress")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("srcutils")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surfaces")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")
ADD_SUBDIRECTORY("testsuite")


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/engine ${CMAKE_BINARY_DIR}/engine ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})
ADD_LIBRARY("regina" MODULE ${SOURCES} )

# python modules on linux should not have the "lib" prefix 
# (windows doesn't have a prefix anyway)
SET_TARGET_PROPERTIES(regina PROPERTIES PREFIX "")

# enforce a non-standard suffix if we have one
IF (REGINA_PYTHON_EXTENSION_NONSTANDARD)
  SET_TARGET_PROPERTIES(regina PROPERTIES SUFFIX ".${REGINA_PYTHON_EXTENSION}")
ENDIF (REGINA_PYTHON_EXTENSION_NONSTANDARD)

TARGET_LINK_LIBRARIES(regina ${ENGINE_LIBRARY} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

# regina-python needs special environment variables when running on a
# 64-bit mac with a 32-bit build.
SET (OSX_PYTHON_32 0)
IF (APPLE)
  IF (CMAKE_OSX_ARCHITECTURES STREQUAL "i386")
    SET (OSX_PYTHON_32 1)
  ENDIF (CMAKE_OSX_ARCHITECTURES STREQUAL "i386")
ENDIF (APPLE)

# regina-python script
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/python/regina-python.in"
  "${PROJECT_BINARY_DIR}/python/regina-python"
  @ONLY
)

INSTALL(TARGETS regina LIBRARY DESTINATION ${PYLIBDIR} COMPONENT Runtime)
INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/python/regina-python DESTINATION ${BINDIR} COMPONENT Runtime)

INSTALL(FILES runscript.py DESTINATION ${PKGDATADIR}/internal/scripts COMPONENT Runtime)

