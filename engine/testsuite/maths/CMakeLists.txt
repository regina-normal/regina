# Files to compile
SET ( FILES
  binom.cpp
  cyclotomic.cpp
  laurent.cpp
  laurent2.cpp
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

