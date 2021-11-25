# split 

# Files to compile
SET ( FILES
  sigcensus.cpp
  sigisomorphism.cpp
  signature.cpp
  pysplit.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} split/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
