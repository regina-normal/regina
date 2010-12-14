# split 

# Files to compile
set ( FILES nsignature pysplit)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} split/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
