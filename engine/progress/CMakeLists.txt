# progress

# Files to compile
set ( FILES nprogresstypes)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} progress/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES nprogress.h nprogressmanager.h nprogresstypes.h DESTINATION ${INCLUDEDIR}/progress)
