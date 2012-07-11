# maths

# Files to compile
SET ( FILES matrixops nlargeinteger nlazyinteger nperm3 nperm4 nperm5 nprimes nrational numbertheory permconv )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

