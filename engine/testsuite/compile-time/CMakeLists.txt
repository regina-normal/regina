# Files to compile
SET ( FILES
  binom.cpp
  boolset.cpp
  concepts.cpp
  disctype.cpp
  facenumbering.cpp
  intutils.cpp
  iterators.cpp
  link.cpp
  perm.cpp
  snappea.cpp
  tableview.cpp
  triangulation.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} compile-time/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

