# algebra

# Files to compile
set ( FILES nabeliangroup ngrouppresentation nmarkedabeliangroup pyalgebra )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)

