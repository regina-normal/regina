# algebra

# Files to compile
SET ( FILES nabeliangroup ngrouppresentation nmarkedabeliangroup nxmlalgebrareader nbinaryA5 nbinaryS4 ncellulardata nbilinearform nsvpolynomialring nhomgrouppresentation ncellulardata.init.cc ncellulardata.locators ncellulardata.tests ncellulardata.init.hom ncellulardata.output ncellulardata.init.pi1 ncellulardata.lookups ncellulardata.init.indexing nmvpolynomialring )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

INSTALL(FILES nabeliangroup.h ngrouppresentation.h nmarkedabeliangroup.h nxmlalgebrareader.h nbinaryA5.h nbinaryS4.h ncellulardata.h nbilinearform.h nsvpolynomialring.h nhomgrouppresentation.h nmvpolynomialring.h DESTINATION ${INCLUDEDIR}/algebra)
