# utilities

# Files to compile
SET ( FILES
  base64
  i18nutils
  nbooleans
  nthread
  nxmlcallback
  osutils
  stringutils
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
    flags.h
    i18nutils.h
    intutils.h
    memutils.h
    nbitmask.h
    nbooleans.h
    nlistoncall.h
    nmarkedvector.h
    nmatrix2.h
    nmpi.h
    nproperty.h
    nqitmask.h
    nrational.h
    nsemiweakbase.h
    nsemiweakptr.h
    nsemiweakremnant.h
    nthread.h
    ntrieset.h
    ntritmask.h
    nxmlcallback.h
    nxmlelementreader.h
    osutils.h
    ptrutils.h
    registryutils.h
    sequence.h
    stringutils.h
    stringutils-impl.h
    stringutils.tcc
    xmlutils.h
    DESTINATION ${INCLUDEDIR}/utilities COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
