# split 

# Files to compile
SET ( FILES
  sigcensus.cpp
  sigisomorphism.cpp
  signature.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} split/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    sigcensus.h
    sigisomorphism.h
    signature.h
    DESTINATION "${PKGINCLUDEDIR}/split" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
