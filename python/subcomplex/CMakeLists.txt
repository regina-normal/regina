# subcomplex

# Files to compile
set ( FILES naugtrisolidtorus nblockedsfs nblockedsfsloop nblockedsfspair nblockedsfstriple nl31pillow nlayeredchain nlayeredchainpair nlayeredlensspace nlayeredloop nlayeredsolidtorus nlayeredsurfacebundle nlayering npillowtwosphere npluggedtorusbundle nplugtrisolidtorus nsatannulus nsatblock nsatblocktypes nsatregion nsnappeacensustri nsnappedball nsnappedtwosphere nspiralsolidtorus nstandardtri ntrisolidtorus ntrivialtri ntxicore pysubcomplex)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} subcomplex/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
