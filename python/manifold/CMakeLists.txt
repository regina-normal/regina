# manifold 

# Files to compile
SET ( FILES
  graphloop.cpp
  graphpair.cpp
  graphtriple.cpp
  handlebody.cpp
  lensspace.cpp
  manifold.cpp
  sfs.cpp
  sfsalt.cpp
  simplesurfacebundle.cpp
  snappeacensusmfd.cpp
  torusbundle.cpp
  pymanifold.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
