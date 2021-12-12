# link

# Files to compile
SET ( FILES
  algebra.cpp
  dt.cpp
  examplelink.cpp
  flype.cpp
  gauss.cpp
  homfly.cpp
  jenkins.cpp
  jones.cpp
  link.cpp
  modellinkgraph.cpp
  pass.cpp
  pd.cpp
  reidemeister.cpp
  reidemeister-tangle.cpp
  rewrite.cpp
  sig.cpp
  simplify.cpp
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
    tangle.h
    DESTINATION "${INCLUDEDIR}/link" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
