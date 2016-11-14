# manifold 

# Files to compile
SET ( FILES
  graphloop
  graphpair
  graphtriple
  handlebody
  lensspace
  nmanifold
  nsfs
  nsfsaltset
  nsimplesurfacebundle
  nsnappeacensusmfd
  ntorusbundle
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
    nmanifold.h
    notation.h
    nsfs.h
    nsfsaltset.h
    nsimplesurfacebundle.h
    nsnappeacensusmfd.h
    ntorusbundle.h
    DESTINATION ${INCLUDEDIR}/manifold COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
