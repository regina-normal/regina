# angle

# Files to compile
SET ( FILES
  anglestructure.cpp
  anglestructures.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} angle/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    angleflags.h
    anglestructure.h
    anglestructures.h
    DESTINATION "${INCLUDEDIR}/angle" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
