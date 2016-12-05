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
  layeredsurfacebundle
  layering
  pillowtwosphere
  pluggedtorusbundle
  plugtrisolidtorus
  satannulus
  nsatblock
  satblockstarter
  satblocktypes
  nsatregion
  nsnappeacensustri
  nsnappedball
  nsnappedtwosphere
  nspiralsolidtorus
  nstandardtri
  ntrisolidtorus
  ntrivialtri
  ntxicore
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
    layeredsurfacebundle.h
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
    nsatblock.h
    satblockstarter.h
    nsatblockstarter.h
    satblocktypes.h
    nsatblocktypes.h
    nsatregion.h
    nsnappeacensustri.h
    nsnappedball.h
    nsnappedtwosphere.h
    nspiralsolidtorus.h
    nstandardtri.h
    ntrisolidtorus.h
    ntrivialtri.h
    ntxicore.h
    DESTINATION ${INCLUDEDIR}/subcomplex COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
