# census

# Files to compile
SET ( FILES
  census
  closedprimemin
  compact
  euler
  gluingperms
  gluingpermsearcher2
  gluingpermsearcher3
  gluingpermsearcher4
  hyperbolic
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    census.h
    dim2gluingperms.h
    dim2gluingpermsearcher.h
    dim4gluingperms.h
    dim4gluingpermsearcher.h
    gluingperms.h
    gluingperms-impl.h
    gluingpermsearcher.h
    gluingpermsearcher2.h
    gluingpermsearcher3.h
    gluingpermsearcher4.h
    ncensus.h
    ngenericgluingperms.h
    ngenericgluingperms-impl.h
    ngluingperms.h
    ngluingpermsearcher.h
    DESTINATION ${INCLUDEDIR}/census COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
