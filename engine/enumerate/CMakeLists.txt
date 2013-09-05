# enumerate

# Files to compile
SET ( FILES nenumconstraint ndoubledescription ndoubledescriptor nhilbertcd nhilbertdual nhilbertprimal nmaxadmissible ntreeconstraint ntreelp ntreetraversal ntypetrie )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

IF (NOT EXCLUDE_NORMALIZ)
ADD_SUBDIRECTORY("normaliz")
ENDIF (NOT EXCLUDE_NORMALIZ)

SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES nenumconstraint.h ndoubledescription.h ndoubledescription.tcc ndoubledescriptor.h ndoubledescriptor.tcc nhilbertcd.h nhilbertcd.tcc nhilbertdual.h nhilbertdual.tcc nhilbertprimal.h nhilbertprimal.tcc nmaxadmissible.h nmaxadmissible.tcc ntreeconstraint.h ntreeconstraint.tcc ntreelp.h ntreelp.tcc ntreetraversal.h ntreetraversal.tcc ntypetrie.h ordering.h DESTINATION ${INCLUDEDIR}/enumerate COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
