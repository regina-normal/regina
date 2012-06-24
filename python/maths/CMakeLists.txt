# maths 

# Files to compile
SET ( FILES matrixops nlargeinteger nlazyinteger nmatrixint nmatrix2 nperm3 nperm4 nperm5 nprimes nrational numbertheory permconv pymaths)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
