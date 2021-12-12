# subcomplex

# Files to compile
SET ( FILES
  augtrisolidtorus.cpp
  blockedsfs.cpp
  blockedsfsloop.cpp
  blockedsfspair.cpp
  blockedsfstriple.cpp
  layeredchain.cpp
  layeredchainpair.cpp
  layeredlensspace.cpp
  layeredloop.cpp
  layeredsolidtorus.cpp
  layeredtorusbundle.cpp
  layering.cpp
  pillowtwosphere.cpp
  pluggedtorusbundle.cpp
  plugtrisolidtorus.cpp
  satannulus.cpp
  satblock.cpp
  satblocktypes.cpp
  satregion.cpp
  snappeacensustri.cpp
  snappedball.cpp
  snappedtwosphere.cpp
  spiralsolidtorus.cpp
  standardtri.cpp
  trisolidtorus.cpp
  trivialtri.cpp
  txicore.cpp
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
    satblocktypes.h
    satregion.h
    satregion-impl.h
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
