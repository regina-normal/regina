# packet

# Files to compile
SET ( FILES
  attachment.cpp
  open.cpp
  packet.cpp
  script.cpp
  text.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} packet/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    attachment.h
    container.h
    packet.h
    packet-impl.h
    pdf.h
    packettype.h
    script.h
    text.h
    DESTINATION "${INCLUDEDIR}/packet" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
