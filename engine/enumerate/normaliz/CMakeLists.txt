# enumerate/normaliz

# Files to compile
SET ( FILES
  HilbertSeries
  cone_property
  libnormaliz-templated
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/normaliz/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

# Normaliz headers should not be shipped: these are for internal use only.
