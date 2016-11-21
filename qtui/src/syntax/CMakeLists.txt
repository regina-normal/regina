SET ( FILES
  abstracthighlighter.cpp
  context.cpp
  contextswitch.cpp
  definition.cpp
  foldingregion.cpp
  format.cpp
  keywordlist.cpp
  ksyntaxhighlighting_logging.cpp
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

INSTALL(FILES
  alert.xml
  alert_indent.xml
  default.theme
  modelines.xml
  python.xml
  DESTINATION ${PKGDATADIR}/syntax)
