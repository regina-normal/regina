# census

# Files to compile
set ( FILES closedprimemin compact ncensus nfacepairing ngluingperms ngluingpermsearcher)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} census/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( CENSUS_SOURCES ${SOURCES} PARENT_SCOPE)
