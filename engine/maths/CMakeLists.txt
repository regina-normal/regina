# maths 

# Files to compile
SET ( FILES
  approx
  matrixops
  ncyclotomic
  ninteger
  nmatrix2
  nperm2
  nperm3
  nperm4
  nperm5
  nprimes
  nrational
  nray
  numbertheory
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
    matrixops.h
    ncyclotomic.h
    ninteger.h
    nlaurent.h
    nlaurent2.h
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
    DESTINATION ${INCLUDEDIR}/maths COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
