# triangulation 

# Files to compile
set ( FILES cover crushtri decompose homology homotopy hydrate insertlayered isomorphic nedge nexampletriangulation nface nfacepair nhomologicaldata nisomorphism ntetrahedron ntriangulation nvertex nxmltrireader reorder signature simplify simplifyglobal skeleton subdivide surfaces turaevviro)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES nboundarycomponent.h ncomponent.h nedge.h nexampletriangulation.h nface.h nfacepair.h nhomologicaldata.h nisomorphism.h nperm.h npermit.h ntetface.h ntetrahedron.h ntriangulation.h nvertex.h nxmltrireader.h DESTINATION ${INCLUDEDIR}/triangulation)
