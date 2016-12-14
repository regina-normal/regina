# generic 

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    boundarycomponent.h
    component.h
    face.h
    facenumbering.h
    facetpairing.h
    facetspec.h
    isomorphism.h
    simplex.h
    triangulation.h
    xmltrireader.h
    DESTINATION ${INCLUDEDIR}/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
