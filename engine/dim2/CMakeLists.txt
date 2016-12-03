
if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dim2boundarycomponent.h
    dim2component.h
    dim2edge.h
    dim2edgepairing.h
    dim2exampletriangulation.h
    dim2isomorphism.h
    dim2triangle.h
    dim2triangulation.h
    dim2vertex.h
    DESTINATION ${INCLUDEDIR}/dim2 COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
