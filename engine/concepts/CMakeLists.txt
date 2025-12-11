# concepts

# No files to compile - concepts are all defined in headers.

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    core.h
    io.h
    iterator.h
    maths.h
    packet.h
    DESTINATION "${PKGINCLUDEDIR}/concepts" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
