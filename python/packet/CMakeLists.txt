# packet

# Files to compile
SET ( FILES
  ncontainer
  npacket
  pdf
  ntext
  packettype
  pypacket
  script
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} packet/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)
