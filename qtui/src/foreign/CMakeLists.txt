SET ( FILES
  csvsurfacehandler
  dehydrationhandler
  exportdialog
  importdialog
  isosighandler
  orbhandler
  pdfhandler
  pythonhandler
  recogniserhandler
  reginahandler
  snappeahandler
  sourcehandler
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} foreign/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
