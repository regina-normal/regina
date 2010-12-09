# census

# Files to compile
set ( FILES closedprimemin compact ncensus nfacepairing ngluingperms ngluingpermsearcher)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} census/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES ncensus.h nfacepairing.h ngluingperms.h ngluingpermsearcher.h DESTINATION ${INCLUDEDIR}/census)
