# generic 

ADD_SUBDIRECTORY("alias")
ADD_SUBDIRECTORY("detail")

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    component.h
    dimtraits.h
    face.h
    facetpairing.h
    isomorphism.h
    nfacetspec.h
    simplex.h
    triangulation.h
    DESTINATION ${INCLUDEDIR}/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
