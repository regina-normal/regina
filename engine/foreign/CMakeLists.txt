# foreign

# Files to compile
set ( FILES csvsurfacelist dehydration orb pdf snappea )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} foreign/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( FOREIGN_SOURCES ${SOURCES} PARENT_SCOPE)
