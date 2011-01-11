# file 

# Files to compile
SET ( FILES nfile nfileinfo nglobaldirs nresources nxmlcallback nxmlfile)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} file/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES  nfile.h nfileinfo.h nfilepropertyreader.h nglobaldirs.h nresources.h nxmlcallback.h nxmlelementreader.h nxmlfile.h DESTINATION ${INCLUDEDIR}/file)
