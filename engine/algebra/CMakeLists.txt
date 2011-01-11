# algebra

# Files to compile
SET ( FILES nabeliangroup ngrouppresentation nmarkedabeliangroup nxmlalgebrareader )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} algebra/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

INSTALL(FILES nabeliangroup.h ngrouppresentation.h nmarkedabeliangroup.h nxmlalgebrareader.h DESTINATION ${INCLUDEDIR}/algebra)
