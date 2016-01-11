# maths

# Files to compile
SET ( FILES
  matrixops
  ninteger
  nperm
  nperm2
  nperm3
  nperm4
  nperm5
  nprimes
  nrational
  numbertheory
  permconv
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

