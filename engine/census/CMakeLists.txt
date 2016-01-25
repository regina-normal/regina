# census

# Files to compile
SET ( FILES
  closedprimemin
  compact
  collapsedchain
  dim2census
  dim2gluingpermsearcher
  enumerationdb
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
    dim2census.h
    dim2edgepairing.h
    dim2gluingperms.h
    dim2gluingpermsearcher.h
    ncensus.h
    nfacepairing.h
    ngenericfacetpairing.h
    ngenericfacetpairing-impl.h
    ngenericfacetpairing.tcc
    ngenericgluingperms.h
    ngenericgluingperms-impl.h
    ngenericgluingperms.tcc
    ngluingperms.h
    ngluingpermsearcher.h
    DESTINATION ${INCLUDEDIR}/census COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
