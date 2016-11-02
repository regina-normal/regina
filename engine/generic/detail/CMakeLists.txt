# generic/detail

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    boundarycomponent-impl.h
    canonical-impl.h
    component.h
    face.h
    facenumbering.h
    facetpairing.h
    facetpairing-impl.h
    isomorphism.h
    isosig-impl.h
    simplex.h
    skeleton-impl.h
    triangulation.h
    xmltrireader.h
    DESTINATION ${INCLUDEDIR}/generic/detail COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
