# link

# Files to compile
SET ( FILES
  link
  xmllinkreader
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} link/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    link.h
    xmllinkreader.h
    DESTINATION ${INCLUDEDIR}/link COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
