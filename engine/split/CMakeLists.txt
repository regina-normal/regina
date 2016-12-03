# split 

# Files to compile
SET ( FILES
  sigcensus
  sigisomorphism
  signature
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} split/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    nsigcensus.h
    nsigisomorphism.h
    nsignature.h
    sigcensus.h
    sigisomorphism.h
    signature.h
    DESTINATION ${INCLUDEDIR}/split COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
