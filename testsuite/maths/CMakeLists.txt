# maths

# Files to compile
SET ( FILES
  integer
  matrixops
  nperm
  nperm2
  nperm3
  nperm4
  nperm5
  numbertheory
  permconv
  primes
  rational
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

