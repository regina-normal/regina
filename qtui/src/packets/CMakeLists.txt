SET ( FILES
  anglescreator
  anglesui
  compatcanvas
  containerui
  eltmovedialog3
  eltmovedialog4
  facetgraphtab
  filtercomb
  filtercreator
  filterprop
  gaprunner
  groupwidget
  hypercompatui
  hypercoordinateui
  hypercreator
  hypermatchingui
  hypersummaryui
  hyperui
  linkcodes
  linkcreator
  linkcrossings
  linkgraph
  linkmovedialog
  linkpoly
  linkui
  pdfui
  scriptui
  skeletonwindow
  snappeaalgebra
  snappeacomponents
  snappeacreator
  snappeafile
  snappeagluings
  snappeashapes
  snappeaui
  surfacescompatui
  surfacescoordinateui
  surfacescreator
  surfacesmatchingui
  surfacessummaryui
  surfacesui
  textui
  tri2creator
  tri2gluings
  tri2skeleton
  tri2ui
  tri3algebra
  tri3creator
  tri3composition
  tri3gluings
  tri3skeleton
  tri3snappea
  tri3surfaces
  tri3ui
  tri4algebra
  tri4creator
  tri4gluings
  tri4skeleton
  tri4ui
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} packets/${FILE})
ENDFOREACH(FILE)

SET ( FILES
  generictriui
  )

IF(NOT LOWDIMONLY)
  FOREACH( FILE ${FILES} )
    SET( SOURCES ${SOURCES} packets/${FILE})
  ENDFOREACH(FILE)
ENDIF(NOT LOWDIMONLY)

SET( SOURCES ${SOURCES} PARENT_SCOPE)
