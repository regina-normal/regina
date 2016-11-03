# census

# Files to compile
SET ( FILES
  dim2census
  dim4census
  facetpairing2
  facetpairing3
  facetpairing4
  ncensus
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

