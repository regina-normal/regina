# Files to compile
SET ( FILES
  snappy_get_gluing_equations
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/snappy/${SOURCE_FILE})
  SET ( SOURCES_C_AS_CPP ${SOURCES_C_AS_CPP} snappea/snappy/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)
SET(SOURCES_C_AS_CPP ${SOURCES_C_AS_CPP} PARENT_SCOPE)

# Snappy headers should not be shipped: these are for internal use only.
