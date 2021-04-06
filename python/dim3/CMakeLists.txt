# dim3 

# Files to compile
SET ( FILES
  boundarycomponent3.cpp
  component3.cpp
  edge3.cpp
  example3.cpp
  facetpairing3.cpp
  facepair.cpp
  homologicaldata.cpp
  isomorphism3.cpp
  tetrahedron3.cpp
  triangle3.cpp
  triangulation3.cpp
  vertex3.cpp
  pytriangulation.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
