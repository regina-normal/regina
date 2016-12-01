# Files to compile
SET ( FILES
  context.cpp
  contextswitch.cpp
  definition.cpp
  format.cpp
  keywordlist.cpp
  repository.cpp
  rule.cpp
  state.cpp
  theme.cpp
  themedata.cpp
  )

# Prepend folder name
FOREACH( SOURCE_FILE ${FILES} )
  SET( SOURCES ${SOURCES} syntax/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)

# Install syntax highlighting definitions
INSTALL(FILES
  alert.xml
  alert_indent.xml
  default.theme
  modelines.xml
  python.xml
  DESTINATION ${PKGDATADIR}/syntax)

# Install development headers
if (${REGINA_INSTALL_DEV})
  INSTALL(FILES
    context_p.h
    contextswitch_p.h
    definition.h
    definition_p.h
    definitionref_p.h
    format.h
    format_p.h
    keywordlist_p.h
    matcher.h
    repository.h
    repository_p.h
    rule_p.h
    state.h
    state_p.h
    textstyledata_p.h
    theme.h
    themedata_p.h
    DESTINATION ${INCLUDEDIR}/syntax COMPONENT Development)
endif (${REGINA_INSTALL_DEV})
