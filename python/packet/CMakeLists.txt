# packet

# Files to compile
SET ( FILES
  container.cpp
  packet.cpp
  packetlistener.cpp
  packettype.cpp
  pdf.cpp
  pypacket.cpp
  script.cpp
  text.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} packet/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)
