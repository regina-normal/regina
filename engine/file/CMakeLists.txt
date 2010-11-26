# file 

# Files to compile
set ( FILES nfile nfileinfo nglobaldirs nresources nxmlcallback nxmlfile)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} file/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( FILE_SOURCES ${SOURCES} PARENT_SCOPE)
