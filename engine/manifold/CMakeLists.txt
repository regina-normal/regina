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
  sfsaltset
  simplesurfacebundle
  snappeacensusmfd
  torusbundle
  order
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    graphloop.h
    ngraphloop.h
    graphpair.h
    ngraphpair.h
    graphtriple.h
    ngraphtriple.h
    handlebody.h
    nhandlebody.h
    lensspace.h
    nlensspace.h
    manifold.h
    nmanifold.h
    notation.h
    sfs.h
    nsfs.h
    sfsaltset.h
    nsfsaltset.h
    simplesurfacebundle.h
    nsimplesurfacebundle.h
    snappeacensusmfd.h
    nsnappeacensusmfd.h
    torusbundle.h
    ntorusbundle.h
    DESTINATION ${INCLUDEDIR}/manifold COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
