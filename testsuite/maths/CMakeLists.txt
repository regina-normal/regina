# maths

# Files to compile
SET ( FILES
  integer
  matrixops
  numbertheory
  perm
  perm2
  perm3
  perm4
  perm5
  permconv
  primes
  rational
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

