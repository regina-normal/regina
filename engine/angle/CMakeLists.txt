# angle

# Files to compile
SET ( FILES
  anglestructure
  anglestructures
  xmlanglestructreader
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} angle/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    anglestructure.h
    anglestructures.h
    nanglestructure.h
    nanglestructurelist.h
    xmlanglestructreader.h
    DESTINATION ${INCLUDEDIR}/angle COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
