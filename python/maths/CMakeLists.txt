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
  matrixint.cpp
  numbertheory.cpp
  perm.cpp
  perm2.cpp
  perm3.cpp
  perm4.cpp
  perm5.cpp
  polynomial.cpp
  primes.cpp
  pymaths.cpp
  rational.cpp
  vectorint.cpp
  vectorlarge.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
