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
    naugtrisolidtorus.h
    blockedsfs.h
    nblockedsfs.h
    blockedsfsloop.h
    nblockedsfsloop.h
    blockedsfspair.h
    nblockedsfspair.h
    blockedsfstriple.h
    nblockedsfstriple.h
    l31pillow.h
    nl31pillow.h
    layeredchain.h
    nlayeredchain.h
    layeredchainpair.h
    nlayeredchainpair.h
    layeredlensspace.h
    nlayeredlensspace.h
    layeredloop.h
    nlayeredloop.h
    layeredsolidtorus.h
    nlayeredsolidtorus.h
    layeredtorusbundle.h
    nlayeredsurfacebundle.h
    layering.h
    nlayering.h
    pillowtwosphere.h
    npillowtwosphere.h
    pluggedtorusbundle.h
    npluggedtorusbundle.h
    plugtrisolidtorus.h
    nplugtrisolidtorus.h
    satannulus.h
    nsatannulus.h
    satblock.h
    nsatblock.h
    satblockstarter.h
    nsatblockstarter.h
    satblocktypes.h
    nsatblocktypes.h
    satregion.h
    nsatregion.h
    snappeacensustri.h
    nsnappeacensustri.h
    snappedball.h
    nsnappedball.h
    snappedtwosphere.h
    nsnappedtwosphere.h
    spiralsolidtorus.h
    nspiralsolidtorus.h
    standardtri.h
    nstandardtri.h
    trisolidtorus.h
    ntrisolidtorus.h
    trivialtri.h
    ntrivialtri.h
    txicore.h
    ntxicore.h
    DESTINATION ${INCLUDEDIR}/subcomplex COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
