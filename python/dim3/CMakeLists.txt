# dim3 

# Files to compile
SET ( FILES
  nboundarycomponent
  ncomponent
  edge3
  nexampletriangulation
  nfacepair
  nfacepairing
  nhomologicaldata
  nisomorphism
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
