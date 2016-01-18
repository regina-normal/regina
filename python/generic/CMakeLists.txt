# Files to compile
SET ( FILES
  component
  face
  face5
  face6
  face7
  face8
  face9
  face10
  face11
  face12
  face13
  face14
  face15
  facehelper
  facetpairing
  isomorphism
  simplex
  pygeneric
  triangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
