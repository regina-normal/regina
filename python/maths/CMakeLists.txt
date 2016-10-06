# maths 

# Files to compile
SET ( FILES
  matrixops
  cyclotomic
  integer
  largeinteger
  matrix2
  matrixint
  numbertheory
  perm
  perm2
  perm3
  perm4
  perm5
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
