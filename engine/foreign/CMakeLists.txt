# foreign

# Files to compile
SET ( FILES csvsurfacelist dehydration isosig orb pdf snappea )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} foreign/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES csvsurfacelist.h dehydration.h isosig.h orb.h pdf.h snappea.h DESTINATION ${INCLUDEDIR}/foreign)
