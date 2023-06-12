if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    face.h
    facenumber.h
    isomorphism.h
    DESTINATION "${PKGINCLUDEDIR}/triangulation/alias" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
