# split 

# Files to compile
SET ( FILES nsigcensus nsigisomorphism nsignature)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} split/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES nsigcensus.h nsigisomorphism.h nsignature.h DESTINATION ${INCLUDEDIR}/split COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
