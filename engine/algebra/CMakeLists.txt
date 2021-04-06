# algebra

# Files to compile
SET ( FILES
  abeliangroup.cpp
  grouppresentation.cpp
  homgrouppresentation.cpp
  markedabeliangroup.cpp
  xmlalgebrareader.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    abeliangroup.h
    grouppresentation.h
    homgrouppresentation.h
    markedabeliangroup.h
    xmlalgebrareader.h
    DESTINATION "${INCLUDEDIR}/algebra" COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
