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
    fromsiglist-impl.h
    isosig.h
    orb.h
    DESTINATION "${INCLUDEDIR}/foreign" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
