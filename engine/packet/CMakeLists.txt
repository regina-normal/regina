# packet

# Files to compile
SET ( FILES
  ncontainer
  npacket
  npacketlistener
  npdf
  nscript
  ntext
  open
  xmlpacketreader
  xmlpacketreaders
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} packet/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    ncontainer.h
    npacket.h
    npacketlistener.h
    npdf.h
    nscript.h
    ntext.h
    xmlpacketreader.h
    xmlpacketreaders.h
    xmltreeresolver.h
    packetregistry.h
    packetregistry-impl.h
    packettype.h
    DESTINATION ${INCLUDEDIR}/packet COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
