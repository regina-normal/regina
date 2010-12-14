# foreign

# Files to compile
set ( FILES csvsurfacelist dehydration orb pdf pyforeign snappea )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} foreign/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
