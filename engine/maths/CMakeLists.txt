# maths 

# Files to compile
SET ( FILES approx matrixops nfastray nlargeinteger nmatrix2 nperm3 nperm4 nprimes nrational nray numbertheory seedprimes)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL( FILES approx.h matrixops.h nfastray.h nfastvector.h nlargeinteger.h nmatrix.h nmatrix2.h nmatrixint.h nperm3.h nperm4.h nprimes.h nrational.h nray.h numbertheory.h nvector.h nvectordense.h nvectormatrix.h nvectorunit.h DESTINATION ${INCLUDEDIR}/maths)
