# surfaces

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    disc.h
    disctype.h
    normalcoords.h
    normalflags.h
    normalsurface.h
    normalsurfaces.h
    prism.h
    surfacefilter.h
    surfacefiltertype.h
    DESTINATION "${INCLUDEDIR}/surfaces" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
