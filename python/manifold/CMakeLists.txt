# manifold 

# Files to compile
SET ( FILES
  graphloop
  graphpair
  graphtriple
  handlebody
  lensspace
  manifold
  sfs
  simplesurfacebundle
  snappeacensusmfd
  torusbundle
  pymanifold
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
