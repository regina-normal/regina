# normaliz

# Files to compile (taken from libnormaliz' Makefile.classic)
SET ( FILES
  automorph
  reduction
  cone_dual_mode
  project_and_lift
  face_lattice
  descent
  HilbertSeries
  sublattice_representation
  matrix
  nmz_nauty
  cone
  cone_property
  general
  collection
  full_cone
  simplex
  input
  options
  offload_handler
  output
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} libnormaliz/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

# Normaliz headers should not be shipped: these are for internal use only.
