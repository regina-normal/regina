# maths 

# Files to compile
set ( FILES approx matrixops nfastray nlargeinteger nmatrix2 nperm3 nperm4 nprimes nrational nray numbertheory seedprimes)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
install( FILES approx.h matrixops.h nfastray.h nfastvector.h nlargeinteger.h nmatrix.h nmatrix2.h nmatrixint.h nperm3.h nperm4.h nprimes.h nrational.h nray.h numbertheory.h nvector.h nvectordense.h nvectormatrix.h nvectorunit.h DESTINATION ${INCLUDEDIR}/maths)
