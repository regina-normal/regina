# maths 

# Files to compile
SET ( FILES approx matrixops nlargeinteger nlazyinteger nmatrix2 nperm3 nperm4 nperm5 nprimes nrational nray numbertheory seedprimes)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL( FILES approx.h matrixops.h nlargeinteger.h nlazyinteger.h nmatrix.h nmatrix2.h nmatrixint.h nperm3.h nperm4.h nperm5.h nprimes.h nrational.h nray.h numbertheory.h nvector.h permconv.h DESTINATION ${INCLUDEDIR}/maths COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
