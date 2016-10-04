# utilities

# Files to compile
SET ( FILES
  boolset
  base64
  i18nutils
  osutils
  stringutils
  xmlcallback
  xmlutils
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
    flags.h
    i18nutils.h
    intutils.h
    listoncall.h
    markedvector.h
    memutils.h
    nbooleans.h
    osutils.h
    property.h
    ptrutils.h
    qitmask.h
    registryutils.h
    safepointeebase.h
    safeptr.h
    saferemnant.h
    sequence.h
    stringutils.h
    stringutils-impl.h
    trieset.h
    tritmask.h
    xmlcallback.h
    xmlelementreader.h
    xmlutils.h
    DESTINATION ${INCLUDEDIR}/utilities COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
