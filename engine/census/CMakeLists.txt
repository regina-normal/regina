# census

# Files to compile
SET ( FILES closedprimemin compact dim2edgepairing ncensus nfacepairing ngluingperms ngluingpermsearcher)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES dim2edgepairing.h ncensus.h nfacepairing.h ngeneralfacetpairing.h ngeneralfacetpairing.tcc ngluingperms.h ngluingpermsearcher.h DESTINATION ${INCLUDEDIR}/census COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
