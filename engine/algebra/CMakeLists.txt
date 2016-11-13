# algebra

# Files to compile
SET ( FILES
  abeliangroup
  grouppresentation
  homgrouppresentation
  nmarkedabeliangroup
  nxmlalgebrareader
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    abeliangroup.h
    nabeliangroup.h
    grouppresentation.h
    ngrouppresentation.h
    homgrouppresentation.h
    nhomgrouppresentation.h
    markedabeliangroup.h
    nmarkedabeliangroup.h
    nxmlalgebrareader.h
    DESTINATION ${INCLUDEDIR}/algebra COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
