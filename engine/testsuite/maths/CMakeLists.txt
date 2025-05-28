# Files to compile
SET ( FILES
  arrow.cpp
  binom.cpp
  cyclotomic.cpp
  integer.cpp
  laurent.cpp
  laurent2.cpp
  matrix.cpp
  matrixops.cpp
  numbertheory.cpp
  perm.cpp
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

