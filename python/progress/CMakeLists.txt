# progress

# Files to compile
set ( FILES nprogress nprogressmanager pyprogress)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} progress/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
