SET ( SOURCES globalarray safeheldtype modulemain )
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("dim3")
ADD_SUBDIRECTORY("dim4")
ADD_SUBDIRECTORY("file")
ADD_SUBDIRECTORY("foreign")
ADD_SUBDIRECTORY("generic")
ADD_SUBDIRECTORY("hypersurface")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("packet")
ADD_SUBDIRECTORY("progress")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("srcutils")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surfaces")
ADD_SUBDIRECTORY("treewidth")
ADD_SUBDIRECTORY("utilities")
ADD_SUBDIRECTORY("testsuite")

# The module itself is built in the regina/ subdirectory, so that we can
# import the module directly from the source tree if we like.
ADD_SUBDIRECTORY("regina")

# regina-python needs special environment variables when running on a
# 64-bit mac with a 32-bit build.
SET (OSX_PYTHON_32 0)
IF (APPLE)
  IF (CMAKE_OSX_ARCHITECTURES STREQUAL "i386")
    SET (OSX_PYTHON_32 1)
  ENDIF (CMAKE_OSX_ARCHITECTURES STREQUAL "i386")
ENDIF (APPLE)

# The startup script for users:
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/python/regina-python.in"
  "${PROJECT_BINARY_DIR}/python/regina-python"
  @ONLY
)

INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/python/regina-python DESTINATION ${BINDIR} COMPONENT Runtime)

