# triangulation 

# Files to compile
set ( FILES nboundarycomponent ncomponent nedge nexampletriangulation nface nfacepair nhomologicaldata nisomorphism ntetface ntetrahedron ntriangulation nvertex pytriangulation)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
