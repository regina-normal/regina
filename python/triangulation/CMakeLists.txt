# triangulation 

# Files to compile
SET ( FILES nboundarycomponent ncomponent nedge nexampletriangulation nface nfacepair nhomologicaldata nisomorphism ntetface ntetrahedron ntriangulation nvertex pytriangulation)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
