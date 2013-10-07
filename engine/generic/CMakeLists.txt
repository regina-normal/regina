# generic 

# Files to compile
SET ( FILES
  isosig
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} generic/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dimtraits.h
    nfacetspec.h
    ngenericisomorphism.h
    ngenericisomorphism.tcc
    DESTINATION ${INCLUDEDIR}/generic COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
