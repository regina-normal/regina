# algebra

# Files to compile
SET ( FILES
  nabeliangroup
  nbilinearform
  nbinaryA5
  nbinaryS4
  ngrouppresentation
  nhomgrouppresentation
  nmarkedabeliangroup
  nmvpolynomialring
  nsvpolynomialring
  nxmlalgebrareader
  nfibring
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    nabeliangroup.h
    nbilinearform.h
    nbinaryA5.h
    nbinaryS4.h
    ngrouppresentation.h
    nhomgrouppresentation.h
    nmarkedabeliangroup.h
    nmvpolynomialring.h
    nsvpolynomialring.h
    nxmlalgebrareader.h
    nfibring.h
    DESTINATION ${INCLUDEDIR}/algebra COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
