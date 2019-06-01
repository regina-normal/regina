# normaliz

# Files to compile (taken from libnormaliz' Makefile.classic)
SET ( FILES
  cone_and_control
  enumeration
  linear_algebra
  offload_handler
  other_algorithms
  output
  primal
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} libnormaliz/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

# Normaliz headers should not be shipped: these are for internal use only.
