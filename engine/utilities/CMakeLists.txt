# utilities

# Files to compile
SET ( FILES
  boolset.cpp
  base64.cpp
  i18nutils.cpp
  memstream.cpp
  osutils.cpp
  randutils.cpp
  stringutils.cpp
  tightencoding.cpp
  trieset.cpp
  typeutils.cpp
  xmlutils.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} utilities/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    base64.h
    bitmanip.h
    bitmask.h
    boolset.h
    exception.h
    flags.h
    i18nutils.h
    intutils.h
    listview.h
    markedvector.h
    memstream.h
    osutils.h
    qitmask.h
    randutils.h
    sequence.h
    shortarray.h
    sigutils.h
    snapshot.h
    stringutils.h
    stringutils-impl.h
    tableview.h
    tightencoding.h
    tightencoding-impl.h
    topologylock.h
    trieset.h
    typeutils.h
    xmlutils.h
    DESTINATION "${INCLUDEDIR}/utilities" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
