# algebra

# Files to compile
SET ( FILES
  nbilinearform
  nbinarya5
  nbinarys4
  ncellulardata
  ngrouppresentation
  nmarkedabeliangroup
  nmvpolynomialring
  nsvpolynomialring
  ngrouppresentation
  fibring
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

