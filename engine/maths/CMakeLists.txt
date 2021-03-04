# maths 

ADD_SUBDIRECTORY("spec")

# Files to compile
SET ( FILES
  cyclotomic
  integer
  matrix2
  matrixops
  numbertheory
  primes
  rational
  ray
  seedprimes
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL( FILES
    binom.h
    cyclotomic.h
    integer.h
    laurent.h
    laurent2.h
    matrix.h
    matrix2.h
    matrixops.h
    numbertheory.h
    perm.h
    perm-impl.h
    polynomial.h
    primes.h
    rational.h
    ray.h
    vector.h
    DESTINATION "${INCLUDEDIR}/maths" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
