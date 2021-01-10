# link

# Files to compile
SET ( FILES
  algebra
  complement
  dt
  examplelink
  flype
  gauss
  homfly
  jenkins
  jones
  link
  modellinkgraph
  pass
  reidemeister
  reidemeister-tangle
  rewrite
  sig
  simplify
  tangle
  xmllinkreader
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} link/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    data-impl.h
    dt-impl.h
    examplelink.h
    gauss-impl.h
    gauss-tangle-impl.h
    graph.h
    link.h
    modellinkgraph.h
    tangle.h
    xmllinkreader.h
    DESTINATION ${INCLUDEDIR}/link COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
