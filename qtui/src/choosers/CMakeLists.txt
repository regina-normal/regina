SET ( FILES
  boundarycomponentchooser
  cuspchooser
  edgeintchooser
  tetrahedronchooser
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} choosers/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
