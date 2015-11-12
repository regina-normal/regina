# generic 

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    canonical-impl.h
    component.h
    dimtraits.h
    facetpairing.h
    facetpairing-impl.h
    isomorphism.h
    isosig-impl.h
    nfacetspec.h
    simplex.h
    triangulation.h
    DESTINATION ${INCLUDEDIR}/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
