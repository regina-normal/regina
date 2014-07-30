# algebra

# Files to compile
SET ( FILES
  nabeliangroup
  nbilinearform
  nbinaryA5
  nbinaryS4
  ncellulardata
  ncellulardata.init.cc
  ncellulardata.init.hom
  ncellulardata.init.indexing
  ncellulardata.init.pi1
  ncellulardata.locators
  ncellulardata.lookups
  ncellulardata.output
  ncellulardata.tests
  ncellulardata.simpaut
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
    ncellulardata.h
    ngrouppresentation.h
    nhomgrouppresentation.h
    nmarkedabeliangroup.h
    nmvpolynomialring.h
    nsvpolynomialring.h
    nxmlalgebrareader.h
    ncellulardata.simpaut.h
    nfibring.h
    DESTINATION ${INCLUDEDIR}/algebra COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
