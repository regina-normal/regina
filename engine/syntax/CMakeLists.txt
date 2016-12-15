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
if (FALSE AND ${REGINA_INSTALL_DEV})
  INSTALL(FILES
    context.h
    contextswitch.h
    definition.h
    definition_p.h
    definitionref.h
    format.h
    format_p.h
    keywordlist.h
    matcher.h
    repository.h
    repository_p.h
    rule.h
    state.h
    state_p.h
    textstyledata_p.h
    theme.h
    themedata_p.h
    DESTINATION ${INCLUDEDIR}/syntax COMPONENT Development)
endif (FALSE AND ${REGINA_INSTALL_DEV})
