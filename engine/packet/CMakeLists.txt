# packet

# Files to compile
SET ( FILES
  open
  packet
  packetlistener
  pdf
  script
  text
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
    container.h
    ncontainer.h
    npacket.h
    npacketlistener.h
    npdf.h
    nscript.h
    ntext.h
    packet.h
    packetlistener.h
    pdf.h
    xmlpacketreader.h
    xmlpacketreaders.h
    xmltreeresolver.h
    packetregistry.h
    packetregistry-impl.h
    packettype.h
    script.h
    text.h
    DESTINATION ${INCLUDEDIR}/packet COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
