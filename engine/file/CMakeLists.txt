# file 

# Files to compile
SET ( FILES
  fileinfo.cpp
  globaldirs.cpp)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} file/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY(xml)

SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    fileformat.h
    fileinfo.h
    globaldirs.h
    DESTINATION "${INCLUDEDIR}/file" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
