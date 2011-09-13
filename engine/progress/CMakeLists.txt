# progress

# Files to compile
SET ( FILES nprogresstypes)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} progress/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES nprogress.h nprogressmanager.h nprogresstypes.h DESTINATION ${INCLUDEDIR}/progress)
