# enumerate

# Files to compile
SET ( FILES
  ntreeconstraint
  ntreelp
  ntreetraversal
  ntypetrie
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    enumconstraints.h
    doubledescription.h
    doubledescription-impl.h
    nenumconstraint.h
    ndoubledescription.h
    ndoubledescription-impl.h
    nhilbertcd.h
    nhilbertcd-impl.h
    nhilbertdual.h
    nhilbertdual-impl.h
    nhilbertprimal.h
    nhilbertprimal-impl.h
    nmaxadmissible.h
    nmaxadmissible-impl.h
    ntreeconstraint.h
    ntreelp.h
    ntreelp-impl.h
    ntreetraversal.h
    ntreetraversal-impl.h
    ntypetrie.h
    ordering.h
    DESTINATION ${INCLUDEDIR}/enumerate COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
