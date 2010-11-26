# maths 

# Files to compile
set ( FILES approx matrixops nfastray nlargeinteger nmatrix2 nperm3 nperm4 nprimes nrational nray numbertheory seedprimes)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( MATHS_SOURCES ${SOURCES} PARENT_SCOPE)
