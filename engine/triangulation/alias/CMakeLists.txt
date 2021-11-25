if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    face.h
    facenumber.h
    isomorphism.h
    DESTINATION "${INCLUDEDIR}/triangulation/alias" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
