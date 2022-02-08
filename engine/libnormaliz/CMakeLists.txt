# normaliz

# Files to compile.
#
# Currently we exclude: nmz_hash, nmz_integral, nmz_nauty
SET ( FILES
  HilbertSeries.cpp
  automorph.cpp
  chunk.cpp
  collection.cpp
  cone.cpp
  cone_dual_mode.cpp
  cone_property.cpp
  descent.cpp
  face_lattice.cpp
  full_cone.cpp
  general.cpp
  input.cpp
  matrix.cpp
  offload_handler.cpp
  options.cpp
  output.cpp
  project_and_lift.cpp
  reduction.cpp
  signed_dec.cpp
  simplex.cpp
  sublattice_representation.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} libnormaliz/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

# Normaliz headers should not be shipped: these are for internal use only.
