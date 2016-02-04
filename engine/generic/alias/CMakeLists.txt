# generic/alias

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    face.h
    facenumber.h
    simplex.h
    DESTINATION ${INCLUDEDIR}/generic/alias COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
