# dim3 

# Files to compile
SET ( FILES
  boundarycomponent3
  component3
  edge3
  example3
  facetpairing3
  facepair
  homologicaldata
  isomorphism3
  tetrahedron3
  triangle3
  triangulation3
  vertex3
  pytriangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
