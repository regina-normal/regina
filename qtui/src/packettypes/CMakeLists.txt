SET ( FILES
  dim2tricreator
  dim2trigluings
  dim2triskeleton
  dim2triui
  eltmovedialog
  facetgraphtab
  gaprunner
  groupwidget
  nanglestructurecreator
  nanglestructureui
  ncompatcanvas
  ncontainerui
  nnormalsurfacecreator
  nnormalsurfaceui
  npdfui
  nscriptui
  nsnappeaalgebra
  nsnappeacreator
  nsnappeafile
  nsnappeagluings
  nsnappeashapes
  nsnappeaui
  nsurfacecompatui
  nsurfacecoordinateui
  nsurfacefiltercomb
  nsurfacefiltercreator
  nsurfacefilterprop
  nsurfacematchingui
  nsurfacesummaryui
  ntextui
  ntrialgebra
  ntriangulationcreator
  ntriangulationui
  ntricomposition
  ntrigluings
  ntriskeleton
  ntrisnappea
  ntrisurfaces
  skeletonwindow
  snappeacomponents
  )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} packettypes/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
