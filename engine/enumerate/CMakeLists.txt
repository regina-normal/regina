# enumerate

# Files to compile
set ( FILES nenumconstraint ndoubledescription ndoubledescriptor)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( ENUMERATE_SOURCES ${SOURCES} PARENT_SCOPE)
