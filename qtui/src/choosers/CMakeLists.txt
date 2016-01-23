SET ( FILES
  boundarycomponentchooser
  cuspchooser
  edgechooser
  edgeintchooser
  tetrahedronchooser
  trianglechooser
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} choosers/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
