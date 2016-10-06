# maths 

ADD_SUBDIRECTORY("spec")

# Files to compile
SET ( FILES
  approx
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
    approx.h
    cyclotomic.h
    integer.h
    matrix.h
    matrix2.h
    matrixops.h
    ncyclotomic.h
    ninteger.h
    nmatrix.h
    nmatrix2.h
    nmatrixint.h
    nperm.h
    nperm-impl.h
    nperm2.h
    nperm3.h
    nperm4.h
    nperm5.h
    npolynomial.h
    nprimes.h
    nrational.h
    nray.h
    numbertheory.h
    nvector.h
    perm.h
    perm-impl.h
    polynomial.h
    primes.h
    rational.h
    ray.h
    vector.h
    DESTINATION ${INCLUDEDIR}/maths COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
