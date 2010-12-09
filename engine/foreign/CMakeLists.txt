# foreign

# Files to compile
set ( FILES csvsurfacelist dehydration orb pdf snappea )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} foreign/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES csvsurfacelist.h dehydration.h orb.h pdf.h snappea.h DESTINATION ${INCLUDEDIR}/foreign)
