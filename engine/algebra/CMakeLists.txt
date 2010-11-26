# algebra

# Files to compile
set ( FILES nabeliangroup ngrouppresentation nmarkedabeliangroup nxmlalgebrareader )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( ALGEBRA_SOURCES ${SOURCES} PARENT_SCOPE)
