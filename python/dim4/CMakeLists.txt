# dim4 

# Files to compile
SET ( FILES
  dim4boundarycomponent
  dim4component
  edge4
  dim4exampletriangulation
  triangle4
  dim4isomorphism
  pentachoron4
  tetrahedron4
  triangulation4
  vertex4
  pydim4
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim4/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
