SET ( FILES
  anglescreator.cpp
  anglesui.cpp
  attachmentui.cpp
  compatcanvas.cpp
  containerui.cpp
  eltmovedialog3.cpp
  eltmovedialog4.cpp
  facetgraphtab.cpp
  filtercomb.cpp
  filtercreator.cpp
  filterprop.cpp
  gaprunner.cpp
  groupwidget.cpp
  hypercompatui.cpp
  hypercoordinateui.cpp
  hypercreator.cpp
  hypermatchingui.cpp
  hypersummaryui.cpp
  hyperui.cpp
  linkcodes.cpp
  linkcreator.cpp
  linkcrossings.cpp
  linkgraph.cpp
  linkmovedialog.cpp
  linkpoly.cpp
  linkui.cpp
  scriptui.cpp
  skeletonwindow.cpp
  snappeaalgebra.cpp
  snappeacomponents.cpp
  snappeacreator.cpp
  snappeafile.cpp
  snappeagluings.cpp
  snappeashapes.cpp
  snappeaui.cpp
  surfacescompatui.cpp
  surfacescoordinateui.cpp
  surfacescreator.cpp
  surfacesmatchingui.cpp
  surfacessummaryui.cpp
  surfacesui.cpp
  textui.cpp
  tri2creator.cpp
  tri2gluings.cpp
  tri2skeleton.cpp
  tri2ui.cpp
  tri3algebra.cpp
  tri3composition.cpp
  tri3creator.cpp
  tri3gluings.cpp
  tri3skeleton.cpp
  tri3snappea.cpp
  tri3surfaces.cpp
  tri3ui.cpp
  tri4algebra.cpp
  tri4creator.cpp
  tri4gluings.cpp
  tri4skeleton.cpp
  tri4ui.cpp
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} packets/${FILE})
ENDFOREACH(FILE)

SET ( FILES
  generictriui.cpp
  )

IF(NOT LOWDIMONLY)
  FOREACH( FILE ${FILES} )
    SET( SOURCES ${SOURCES} packets/${FILE})
  ENDFOREACH(FILE)
ENDIF(NOT LOWDIMONLY)

SET( SOURCES ${SOURCES} PARENT_SCOPE)
