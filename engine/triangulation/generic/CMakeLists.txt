if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    boundarycomponent.h
    component.h
    face.h
    faceembedding.h
    facetpairing.h
    isomorphism.h
    simplex.h
    triangulation.h
    DESTINATION "${PKGINCLUDEDIR}/triangulation/generic" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
