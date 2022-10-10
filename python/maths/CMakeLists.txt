# maths 

# Files to compile
SET ( FILES
  binom.cpp
  laurent.cpp
  laurent2.cpp
  matrixops.cpp
  cyclotomic.cpp
  integer.cpp
  largeinteger.cpp
  matrix2.cpp
  matrixbool.cpp
  matrixint.cpp
  numbertheory.cpp
  perm.cpp
  perm2.cpp
  perm3.cpp
  perm4.cpp
  perm5.cpp
  perm6.cpp
  perm7.cpp
  polynomial.cpp
  primes.cpp
  pymaths.cpp
  rational.cpp
  vector.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
