# maths 

# Files to compile
set ( FILES matrixops nlargeinteger nmatrix2 nperm3 nperm4 nprimes nrational numbertheory pymaths)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
