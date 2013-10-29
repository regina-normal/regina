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
IF (NOT EXCLUDE_SNAPPEA)
ADD_SUBDIRECTORY("snappea")
ENDIF (NOT EXCLUDE_SNAPPEA)
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("srcutils")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surfaces")
ADD_SUBDIRECTORY("triangulation")
ADD_SUBDIRECTORY("utilities")
ADD_SUBDIRECTORY("testsuite")


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/engine ${CMAKE_BINARY_DIR}/engine ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
ADD_LIBRARY("regina" MODULE ${SOURCES} )

# Python modules on linux should not have the "lib" prefix
# (windows doesn't have a prefix anyway).
SET_TARGET_PROPERTIES(regina PROPERTIES PREFIX "")

# Enforce a non-standard suffix if we have one.
IF (REGINA_PYTHON_EXTENSION_NONSTANDARD)
  SET_TARGET_PROPERTIES(regina PROPERTIES SUFFIX ".${REGINA_PYTHON_EXTENSION}")
ENDIF (REGINA_PYTHON_EXTENSION_NONSTANDARD)

IF (APPLE)
  # Avoid linking with ${PYTHON_LIBRARIES} so we can load the module in a
  # different python build if we so desire.
  # On MacOS this requires the "-undefined dynamic_lookup" linker flag.
  SET_TARGET_PROPERTIES(regina PROPERTIES LINK_FLAGS
    "-undefined dynamic_lookup")
  TARGET_LINK_LIBRARIES(regina ${ENGINE_LIBRARY} ${Boost_LIBRARIES})
ELSEIF (WIN32)
  # On Windows, just link everything in; the boost.python build scripts
  # do this also.
  TARGET_LINK_LIBRARIES(regina ${ENGINE_LIBRARY} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
ELSE ()
  # On Linux and relations, again do not link with ${PYTHON_LIBRARIES}.
  # This should not require any special linker flags.
  TARGET_LINK_LIBRARIES(regina ${ENGINE_LIBRARY} ${Boost_LIBRARIES})
ENDIF ()

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

INSTALL(FILES runscript.py DESTINATION ${PKGDATADIR}/scripts COMPONENT Runtime)

