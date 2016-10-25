# dim3 

# Files to compile
SET ( FILES
  nboundarycomponent
  ncomponent
  nedge
  nexampletriangulation
  nfacepair
  nfacepairing
  nhomologicaldata
  nisomorphism
  ntetrahedron
  ntriangle
  triangulation3
  nvertex
  pytriangulation
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim3/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
