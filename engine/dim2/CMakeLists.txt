# dim2 

# Files to compile
SET ( FILES canonical dim2edge dim2triangle dim2isomorphism dim2triangulation isomorphic nxmldim2trireader signature skeleton )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} dim2/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES dim2boundarycomponent.h dim2component.h dim2edge.h dim2triangle.h dim2isomorphism.h dim2triangulation.h dim2vertex.h nxmldim2trireader.h DESTINATION ${INCLUDEDIR}/dim2 COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
