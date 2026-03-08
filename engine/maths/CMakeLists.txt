# maths 

ADD_SUBDIRECTORY("spec")

# Files to compile
SET ( FILES
  arrow.cpp
  cyclotomic.cpp
  integer.cpp
  matrix2.cpp
  matrixops.cpp
  numbertheory.cpp
  perm.cpp
  permgroup.cpp
  primes.cpp
  rational.cpp
  seedprimes.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL( FILES
    3d.h
    arrow.h
    binom.h
    cyclotomic.h
    forward.h
    integer.h
    laurent.h
    laurent2.h
    matrix.h
    matrix2.h
    matrixops.h
    numbertheory.h
    perm.h
    permgroup.h
    permsn.h
    polynomial.h
    primes.h
    rational.h
    ring.h
    vector.h
    DESTINATION "${PKGINCLUDEDIR}/maths" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
