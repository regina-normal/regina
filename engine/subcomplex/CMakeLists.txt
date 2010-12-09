# subcomplex

# Files to compile
set ( FILES naugtrisolidtorus nblockedsfs nblockedsfsloop nblockedsfspair nblockedsfstriple nl31pillow nlayeredchain nlayeredchainpair nlayeredlensspace nlayeredloop nlayeredsolidtorus nlayeredsurfacebundle nlayering npillowtwosphere npluggedtorusbundle nplugtrisolidtorus nsatannulus nsatblock nsatblockstarter nsatblocktypes nsatregion nsnappeacensustri nsnappedball nsnappedtwosphere nspiralsolidtorus nstandardtri ntrisolidtorus ntrivialtri ntxicore )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} subcomplex/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES naugtrisolidtorus.h nblockedsfs.h nblockedsfsloop.h nblockedsfspair.h nblockedsfstriple.h nl31pillow.h nlayeredchain.h nlayeredchainpair.h nlayeredlensspace.h nlayeredloop.h nlayeredsolidtorus.h nlayeredsurfacebundle.h nlayering.h npillowtwosphere.h npluggedtorusbundle.h nplugtrisolidtorus.h nsatannulus.h nsatblock.h nsatblockstarter.h nsatblocktypes.h nsatregion.h nsnappeacensustri.h nsnappedball.h nsnappedtwosphere.h nspiralsolidtorus.h nstandardtri.h ntrisolidtorus.h ntrivialtri.h ntxicore.h DESTINATION ${INCLUDEDIR}/subcomplex)
