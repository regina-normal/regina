# maths

# Files to compile
SET ( FILES
  binom.cpp
  cyclotomic.cpp
  integer.cpp
  laurent.cpp
  laurent2.cpp
  matrix.cpp
  matrixops.cpp
  numbertheory.cpp
  perm.cpp
  perm2.cpp
  perm3.cpp
  perm4.cpp
  perm5.cpp
  perm6.cpp
  perm7.cpp
  permconv.cpp
  polynomial.cpp
  primes.cpp
  rational.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

