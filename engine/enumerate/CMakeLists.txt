# enumerate

# Files to compile
SET ( FILES
  nenumconstraint
  ndoubledescription
  ndoubledescriptor
  nhilbertcd
  nhilbertdual
  nhilbertprimal
  nmaxadmissible
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
    nenumconstraint.h
    ndoubledescription.h
    ndoubledescription-impl.h
    ndoubledescription.tcc
    ndoubledescriptor.h
    ndoubledescriptor.tcc
    nhilbertcd.h
    nhilbertcd-impl.h
    nhilbertcd.tcc
    nhilbertdual.h
    nhilbertdual-impl.h
    nhilbertdual.tcc
    nhilbertprimal.h
    nhilbertprimal-impl.h
    nhilbertprimal.tcc
    nmaxadmissible.h
    nmaxadmissible-impl.h
    nmaxadmissible.tcc
    ntreeconstraint.h
    ntreelp.h
    ntreelp-impl.h
    ntreelp.tcc
    ntreetraversal.h
    ntreetraversal-impl.h
    ntreetraversal.tcc
    ntypetrie.h
    ordering.h
    DESTINATION ${INCLUDEDIR}/enumerate COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
