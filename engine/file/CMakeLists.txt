# file 

# Files to compile
SET ( FILES
  nfileinfo
  nglobaldirs)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} file/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    nfileinfo.h
    nglobaldirs.h
    DESTINATION ${INCLUDEDIR}/file COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
