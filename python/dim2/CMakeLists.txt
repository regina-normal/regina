# dim2 

# Files to compile
SET ( FILES dim2boundarycomponent dim2component dim2edge dim2triangle dim2isomorphism dim2triangleedge dim2triangulation dim2vertex pydim2 )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim2/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
