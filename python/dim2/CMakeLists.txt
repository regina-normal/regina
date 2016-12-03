# dim2 

# Files to compile
SET ( FILES
  boundarycomponent2
  component2
  edge2
  example2
  triangle2
  isomorphism2
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
