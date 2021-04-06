SET ( FILES
  boundary3chooser.cpp
  boundary4chooser.cpp
  cuspchooser.cpp
  edgeintchooser.cpp
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} choosers/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
