# snappea 

# Files to compile
set ( FILES nsnappeatriangulation uimessages)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} snappea/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( ${SOURCES} PARENT_SCOPE)
install(FILES nsnappeatriangulation.h DESTINATION ${INCLUDEDIR}/snappea)
