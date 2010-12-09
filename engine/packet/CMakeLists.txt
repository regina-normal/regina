# packet

# Files to compile
set ( FILES ncontainer npacket npacketlistener npackettype npdf nscript ntext nxmlpacketreader nxmlpacketreaders)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} packet/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set(SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES ncontainer.h npacket.h npacketlistener.h npdf.h nscript.h ntext.h nxmlpacketreader.h nxmlpacketreaders.h packetregistry.h DESTINATION ${INCLUDEDIR}/packet)
