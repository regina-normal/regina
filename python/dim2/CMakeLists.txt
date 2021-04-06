# dim2 

# Files to compile
SET ( FILES
  boundarycomponent2.cpp
  component2.cpp
  edge2.cpp
  example2.cpp
  triangle2.cpp
  isomorphism2.cpp
  triangulation2.cpp
  vertex2.cpp
  pydim2.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim2/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
