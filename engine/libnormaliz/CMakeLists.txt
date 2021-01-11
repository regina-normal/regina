# normaliz

# Files to compile.
#
# Currently we exclude: nmz_integral, nmz_nauty
SET ( FILES
  HilbertSeries
  automorph
  collection
  cone
  cone_dual_mode
  cone_property
  descent
  face_lattice
  full_cone
  general
  input
  matrix
  offload_handler
  options
  output
  project_and_lift
  reduction
  simplex
  sublattice_representation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} libnormaliz/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

# Normaliz headers should not be shipped: these are for internal use only.
