# concepts

# No files to compile - concepts are all defined in headers.

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    core.h
    DESTINATION "${PKGINCLUDEDIR}/concepts" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
