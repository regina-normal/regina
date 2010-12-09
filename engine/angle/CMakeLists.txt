# angle

# Files to compile
set ( FILES nanglestructure nanglestructurelist nxmlanglestructreader )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} angle/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES nanglestructure.h nanglestructurelist.h nxmlanglestructreader.h DESTINATION ${INCLUDEDIR}/angle)
