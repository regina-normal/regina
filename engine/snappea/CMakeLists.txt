# snappea 

# Files to compile
SET ( FILES nsnappeatriangulation uimessages)


# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} snappea/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

ADD_SUBDIRECTORY(kernel)


# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

INSTALL(FILES nsnappeatriangulation.h DESTINATION ${INCLUDEDIR}/snappea)

