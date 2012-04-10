# dim4 

# Files to compile
SET ( FILES canonical dim4boundarycomponent dim4edge dim4exampletriangulation dim4face dim4isomorphism dim4pentachoron dim4tetrahedron dim4triangulation dim4vertex homology homotopy isomorphic nxmldim4trireader signature simplify simplifyglobal skeleton )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim4/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES dim4boundarycomponent.h dim4component.h dim4edge.h dim4exampletriangulation.h dim4face.h dim4isomorphism.h dim4pentachoron.h dim4tetrahedron.h dim4triangulation.h dim4vertex.h nxmldim4trireader.h DESTINATION ${INCLUDEDIR}/dim4 COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
