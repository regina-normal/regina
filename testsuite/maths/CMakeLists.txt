# maths

# Files to compile
SET ( FILES
  integer.cpp
  matrix.cpp
  matrixops.cpp
  perm.cpp
  perm2.cpp
  perm3.cpp
  perm4.cpp
  perm5.cpp
  perm6.cpp
  perm7.cpp
  primes.cpp
  rational.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

