# packet

# Files to compile
SET ( FILES ncontainer npacket npacketlistener npackettype npdf nscript ntext nxmlpacketreader nxmlpacketreaders)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} packet/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES ncontainer.h npacket.h npacketlistener.h npdf.h nscript.h ntext.h nxmlpacketreader.h nxmlpacketreaders.h packetregistry.h DESTINATION ${INCLUDEDIR}/packet)
