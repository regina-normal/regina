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
  satblock
  satblocktypes
  satregion
  snappeacensustri
  nsnappedball
  nsnappedtwosphere
  nspiralsolidtorus
  nstandardtri
  ntrisolidtorus
  ntrivialtri
  ntxicore
  pysubcomplex
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} subcomplex/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
