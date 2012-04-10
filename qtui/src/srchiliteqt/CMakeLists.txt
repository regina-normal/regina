SET ( FILES GNUSyntaxHighlighter Qt4SyntaxHighlighter Qt4TextFormatter Qt4TextFormatterFactory QtColorMap TextFormatter TextFormatterFactory )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} srchiliteqt/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
