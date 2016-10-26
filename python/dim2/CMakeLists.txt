# dim2 

# Files to compile
SET ( FILES
  dim2boundarycomponent
  dim2component
  edge2
  dim2exampletriangulation
  triangle2
  dim2isomorphism
  triangulation2
  vertex2
  pydim2
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim2/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
