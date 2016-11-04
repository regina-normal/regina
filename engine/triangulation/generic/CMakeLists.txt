if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    example.h
    DESTINATION ${INCLUDEDIR}/triangulation/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
