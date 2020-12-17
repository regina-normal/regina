# maths

# Files to compile
SET ( FILES
  cyclotomic
  integer
  laurent
  laurent2
  matrixops
  numbertheory
  perm
  perm2
  perm3
  perm4
  perm5
  permconv
  polynomial
  primes
  rational
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

