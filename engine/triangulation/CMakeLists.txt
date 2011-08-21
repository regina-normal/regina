# triangulation 

# Files to compile
SET ( FILES cover crushtri decompose homology homotopy hydrate insertlayered isomorphic nedge nexampletriangulation nface nfacepair nhomologicaldata nisomorphism ntetrahedron ntriangulation nvertex nxmltrireader reorder signature simplify simplifyglobal skeleton subdivide surfaces turaevviro)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} triangulation/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES nboundarycomponent.h ncomponent.h nedge.h nexampletriangulation.h nface.h nfacepair.h nhomologicaldata.h nisomorphism.h nperm.h npermit.h ntetface.h ntetrahedron.h ntriangulation.h nvertex.h nxmltrireader.h DESTINATION ${INCLUDEDIR}/triangulation)
