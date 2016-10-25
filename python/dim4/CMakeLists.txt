# dim4 

# Files to compile
SET ( FILES
  dim4boundarycomponent
  dim4component
  dim4edge
  dim4exampletriangulation
  dim4triangle
  dim4isomorphism
  dim4pentachoron
  dim4tetrahedron
  triangulation4
  dim4vertex
  pydim4
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim4/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
