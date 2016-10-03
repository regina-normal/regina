# progress

# Files to compile
SET ( FILES
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} progress/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    nprogresstracker.h
    progresstracker.h
    DESTINATION ${INCLUDEDIR}/progress COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
