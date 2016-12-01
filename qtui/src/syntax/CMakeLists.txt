SET ( FILES
  abstracthighlighter.cpp
  context.cpp
  contextswitch.cpp
  definition.cpp
  format.cpp
  keywordlist.cpp
  qtmatcher.cpp
  repository.cpp
  rule.cpp
  state.cpp
  syntaxhighlighter.cpp
  theme.cpp
  themedata.cpp
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
