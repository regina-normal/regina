# census

# Files to compile
SET ( FILES closedprimemin compact ncensus nfacepairing ngluingperms ngluingpermsearcher)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} census/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_TYPE} STREQUAL FullInstall)
  INSTALL(FILES ncensus.h nfacepairing.h ngluingperms.h ngluingpermsearcher.h DESTINATION ${INCLUDEDIR}/census COMPONENT Development)
endif (${REGINA_INSTALL_TYPE} STREQUAL FullInstall)
