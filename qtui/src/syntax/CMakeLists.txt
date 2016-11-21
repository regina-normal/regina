SET ( FILES
  abstracthighlighter.cpp
  context.cpp
  contextswitch.cpp
  definition.cpp
  definitiondownloader.cpp
  foldingregion.cpp
  format.cpp
  htmlhighlighter.cpp
  keywordlist.cpp
  matchresult.cpp
  repository.cpp
  rule.cpp
  state.cpp
  syntaxhighlighter.cpp
  theme.cpp
  themedata.cpp
  wildcardmatcher.cpp
  )
FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
ENDFOREACH(FILE)

SET( SOURCES ${SOURCES} PARENT_SCOPE)

INSTALL(FILES python.xml default.theme DESTINATION ${PKGDATADIR}/syntax)
