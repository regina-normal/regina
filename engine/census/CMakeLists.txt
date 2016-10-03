# census

# Files to compile
SET ( FILES
  closedprimemin
  compact
  dim2gluingpermsearcher
  dim4gluingpermsearcher
  euler
  hyperbolic
  ncensus
  ngenericgluingperms
  ngluingpermsearcher
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dim2gluingperms.h
    dim2gluingpermsearcher.h
    dim4gluingperms.h
    dim4gluingpermsearcher.h
    ncensus.h
    ngenericgluingperms.h
    ngenericgluingperms-impl.h
    ngluingperms.h
    ngluingpermsearcher.h
    DESTINATION ${INCLUDEDIR}/census COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
