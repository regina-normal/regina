# file 

# Files to compile
SET ( FILES
  fileinfo
  globaldirs)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} file/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    fileinfo.h
    globaldirs.h
    nfileinfo.h
    nglobaldirs.h
    DESTINATION ${INCLUDEDIR}/file COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
