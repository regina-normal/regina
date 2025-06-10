# link

# Files to compile
SET ( FILES
  affineindex.cpp
  alexander.cpp
  algebra.cpp
  arrow.cpp
  complement.cpp
  dt.cpp
  examplelink.cpp
  flype.cpp
  gauss.cpp
  group.cpp
  homfly.cpp
  jenkins.cpp
  jones.cpp
  knotplot.cpp
  link.cpp
  modellinkgraph.cpp
  operations.cpp
  pass.cpp
  pd.cpp
  plantri.cpp
  reidemeister.cpp
  reidemeister-tangle.cpp
  rewrite.cpp
  sig.cpp
  simplify.cpp
  spatiallink.cpp
  tangle.cpp
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
    jenkins-impl.h
    link.h
    modellinkgraph.h
    modellinkgraph-impl.h
    pd-impl.h
    spatiallink.h
    tangle.h
    DESTINATION "${PKGINCLUDEDIR}/link" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
