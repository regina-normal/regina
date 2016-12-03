if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    face.h
    facenumber.h
    isomorphism.h
    simplex.h
    DESTINATION ${INCLUDEDIR}/triangulation/alias COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
