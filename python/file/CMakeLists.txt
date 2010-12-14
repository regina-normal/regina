# file 

# Files to compile
set ( FILES nfileinfo nglobaldirs nxmlfile pyfile)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} file/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
