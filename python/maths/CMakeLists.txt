# maths 

# Files to compile
SET ( FILES
  matrixops
  cyclotomic
  integer
  largeinteger
  matrix2
  matrixint
  nperm
  nperm2
  nperm3
  nperm4
  nperm5
  numbertheory
  polynomial
  primes
  pymaths
  rational
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
