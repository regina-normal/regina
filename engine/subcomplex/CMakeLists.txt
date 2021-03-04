# subcomplex

# Files to compile
SET ( FILES
  augtrisolidtorus
  blockedsfs
  blockedsfsloop
  blockedsfspair
  blockedsfstriple
  l31pillow
  layeredchain
  layeredchainpair
  layeredlensspace
  layeredloop
  layeredsolidtorus
  layeredtorusbundle
  layering
  pillowtwosphere
  pluggedtorusbundle
  plugtrisolidtorus
  satannulus
  satblock
  satblockstarter
  satblocktypes
  satregion
  snappeacensustri
  snappedball
  snappedtwosphere
  spiralsolidtorus
  standardtri
  trisolidtorus
  trivialtri
  txicore
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} subcomplex/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    augtrisolidtorus.h
    blockedsfs.h
    blockedsfsloop.h
    blockedsfspair.h
    blockedsfstriple.h
    l31pillow.h
    layeredchain.h
    layeredchainpair.h
    layeredlensspace.h
    layeredloop.h
    layeredsolidtorus.h
    layeredtorusbundle.h
    layering.h
    pillowtwosphere.h
    pluggedtorusbundle.h
    plugtrisolidtorus.h
    satannulus.h
    satblock.h
    satblockstarter.h
    satblocktypes.h
    satregion.h
    snappeacensustri.h
    snappedball.h
    snappedtwosphere.h
    spiralsolidtorus.h
    standardtri.h
    trisolidtorus.h
    trivialtri.h
    txicore.h
    DESTINATION "${INCLUDEDIR}/subcomplex" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
