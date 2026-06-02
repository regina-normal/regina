# foreign

# Files to compile
SET ( FILES
  dehydration.cpp
  orb.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} foreign/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    dehydration.h
    isosig.h
    orb.h
    siglist.h
    siglist-impl.h
    DESTINATION "${PKGINCLUDEDIR}/foreign" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
