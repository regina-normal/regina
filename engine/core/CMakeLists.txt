# Files to compile
SET ( FILES engine.cpp )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} core/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    engine.h
    output.h
    DESTINATION "${PKGINCLUDEDIR}/core" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
