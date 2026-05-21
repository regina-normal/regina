if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    face.h
    faceembedding.h
    isomorphism.h
    simplex.h
    DESTINATION "${PKGINCLUDEDIR}/triangulation/generic" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
