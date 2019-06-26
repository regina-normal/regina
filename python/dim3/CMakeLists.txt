# dim3 

# Files to compile
SET ( FILES
  # TODO boundarycomponent3
  # TODO component3
  # TODO edge3
  example3
  # TODO facetpairing3
  # TODO facepair
  # TODO homologicaldata
  isomorphism3
  tetrahedron3
  # TODO triangle3
  triangulation3
  # TODO vertex3
  pytriangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
