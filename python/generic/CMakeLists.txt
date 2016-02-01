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
  facetpairing2
  facetpairing4
  facetpairing5
  facetpairing6
  facetpairing7
  facetpairing8
  facetpairing9
  facetpairing10
  facetpairing11
  facetpairing12
  facetpairing13
  facetpairing14
  facetpairing15
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
