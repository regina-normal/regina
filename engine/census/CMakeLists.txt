# census

# Files to compile
SET ( FILES closedprimemin compact dim2census dim2edgepairing dim2gluingperms dim2gluingpermsearcher ncensus nfacepairing ngluingperms ngluingpermsearcher)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES dim2census.h dim2edgepairing.h dim2gluingperms.h dim2gluingpermsearcher.h ncensus.h nfacepairing.h ngenericfacetpairing.h ngenericfacetpairing.tcc ngenericgluingperms.h ngenericgluingperms.tcc ngluingperms.h ngluingpermsearcher.h DESTINATION ${INCLUDEDIR}/census COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
