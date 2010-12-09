# file 

# Files to compile
set ( FILES nfile nfileinfo nglobaldirs nresources nxmlcallback nxmlfile)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} file/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES  nfile.h nfileinfo.h nfilepropertyreader.h nglobaldirs.h nresources.h nxmlcallback.h nxmlelementreader.h nxmlfile.h DESTINATION ${INCLUDEDIR}/file)
