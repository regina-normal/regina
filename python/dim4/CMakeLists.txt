# dim4 

# Files to compile
SET ( FILES
  boundarycomponent4.cpp
  component4.cpp
  edge4.cpp
  example4.cpp
  triangle4.cpp
  isomorphism4.cpp
  pentachoron4.cpp
  tetrahedron4.cpp
  triangulation4.cpp
  vertex4.cpp
  pydim4.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim4/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
