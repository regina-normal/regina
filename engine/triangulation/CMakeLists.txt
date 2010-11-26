# triangulation 

# Files to compile
set ( FILES cover crushtri decompose homology homotopy hydrate insertlayered isomorphic nedge nexampletriangulation nface nfacepair nhomologicaldata nisomorphism ntetrahedron ntriangulation nvertex nxmltrireader reorder signature simplify simplifyglobal skeleton subdivide surfaces turaevviro)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( TRIANGULATION_SOURCES ${SOURCES} PARENT_SCOPE)
