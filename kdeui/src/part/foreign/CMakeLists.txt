# Engine

SET ( FILES csvsurfacehandler dehydrationhandler exportdialog importdialog orbhandler pdfhandler pythonhandler reginahandler snappeahandler sourcehandler )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} foreign/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
