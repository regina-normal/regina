# census

# Files to compile
SET ( FILES
  census2
  census3
  census4
  facetpairing2
  facetpairing3
  facetpairing4
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

