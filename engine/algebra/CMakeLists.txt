# algebra

# Files to compile
SET ( FILES
  abeliangroup
  grouppresentation
  homgrouppresentation
  binaryA5
  binaryS4
  markedabeliangroup
  xmlalgebrareader
  cellulardata          
  cellulardata.init.hom      
  cellulardata.init.pi1  
  cellulardata.lookups  
  cellulardata.simpaut
  cellulardata.init.cc  
  cellulardata.init.indexing  
  cellulardata.locators
  cellulardata.output   
  cellulardata.tests
  bilinearform
  svpolynomialring
  mvpolynomialring
  fibring
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
    binaryA5.h
    binaryS4.h
    markedabeliangroup.h
    nmarkedabeliangroup.h
    xmlalgebrareader.h
    cellulardata.h  
    cellulardata.simpaut.h
    bilinearform.h
    svpolynomialring.h
    mvpolynomialring.h
    fibring.h
    DESTINATION ${INCLUDEDIR}/algebra COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
