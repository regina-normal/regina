# maths 

# Files to compile
SET ( FILES
  matrixops
  ncyclotomic
  ninteger
  nlargeinteger
  nmatrixint
  nmatrix2
  nperm3
  nperm4
  nperm5
  npolynomial
  nprimes
  nrational
  numbertheory
  permconv
  pymaths
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
