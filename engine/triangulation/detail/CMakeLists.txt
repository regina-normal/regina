if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    boundarycomponent-impl.h
    boundarycomponent.h
    canonical-impl.h
    component.h
    example.h
    face.h
    facenumbering.h
    facetpairing.h
    facetpairing-impl.h
    isomorphism.h
    isosig-impl.h
    simplex.h
    skeleton-impl.h
    strings.h
    triangulation.h
    xmltrireader.h
    DESTINATION ${INCLUDEDIR}/triangulation/detail COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
