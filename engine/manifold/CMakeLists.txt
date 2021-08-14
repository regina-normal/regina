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
  order.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    graphloop.h
    graphpair.h
    graphtriple.h
    handlebody.h
    lensspace.h
    manifold.h
    notation.h
    sfs.h
    sfsalt.h
    simplesurfacebundle.h
    snappeacensusmfd.h
    torusbundle.h
    DESTINATION "${INCLUDEDIR}/manifold" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
