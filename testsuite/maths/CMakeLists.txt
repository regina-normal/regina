# maths

# Files to compile
SET ( FILES matrixops nlargeinteger nperm3 nperm4 nprimes nrational numbertheory )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

