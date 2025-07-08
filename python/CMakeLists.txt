INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/python/pybind11_v${REGINA_PYBIND11_VERSION}")

SET ( SOURCES modulemain.cpp )
ADD_SUBDIRECTORY("algebra")
ADD_SUBDIRECTORY("angle")
ADD_SUBDIRECTORY("census")
ADD_SUBDIRECTORY("dim2")
ADD_SUBDIRECTORY("dim3")
ADD_SUBDIRECTORY("dim4")
ADD_SUBDIRECTORY("enumerate")
ADD_SUBDIRECTORY("file")
ADD_SUBDIRECTORY("foreign")
ADD_SUBDIRECTORY("generic")
ADD_SUBDIRECTORY("hypersurface")
ADD_SUBDIRECTORY("link")
ADD_SUBDIRECTORY("manifold")
ADD_SUBDIRECTORY("maths")
ADD_SUBDIRECTORY("packet")
ADD_SUBDIRECTORY("progress")
ADD_SUBDIRECTORY("snappea")
ADD_SUBDIRECTORY("split")
ADD_SUBDIRECTORY("subcomplex")
ADD_SUBDIRECTORY("surface")
ADD_SUBDIRECTORY("treewidth")
ADD_SUBDIRECTORY("utilities")

ADD_SUBDIRECTORY("testsuite")
ADD_SUBDIRECTORY("sage")

# The module itself is built in the regina/ subdirectory, so that we can
# import the module directly from the source tree if we like.
ADD_SUBDIRECTORY("regina")

# The startup script for users:
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/python/regina-python.in"
  "${PROJECT_BINARY_DIR}/python/regina-python"
  @ONLY
)

INSTALL(PROGRAMS "${PROJECT_BINARY_DIR}/python/regina-python" DESTINATION "${BINDIR}" COMPONENT Runtime)

