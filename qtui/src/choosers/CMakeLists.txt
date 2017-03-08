SET ( FILES
  boundary3chooser
  boundary4chooser
  cuspchooser
  edgeintchooser
  tetintchooser
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} choosers/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
