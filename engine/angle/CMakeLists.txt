# angle

# Files to compile
SET ( FILES nanglestructure nanglestructurelist nxmlanglestructreader )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} angle/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES nanglestructure.h nanglestructurelist.h nxmlanglestructreader.h DESTINATION ${INCLUDEDIR}/angle)
