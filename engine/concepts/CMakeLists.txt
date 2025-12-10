# concepts

# No files to compile - concepts are all defined in headers.

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    arithmetic.h
    core.h
    io.h
    maths.h
    packet.h
    DESTINATION "${PKGINCLUDEDIR}/concepts" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
