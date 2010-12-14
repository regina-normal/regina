# packet

# Files to compile
set ( FILES ncontainer npacket npdf nscript ntext pypacket)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} packet/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set(SOURCES ${SOURCES} PARENT_SCOPE)
