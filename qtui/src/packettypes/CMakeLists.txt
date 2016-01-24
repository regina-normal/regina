SET ( FILES
  dim2tricreator
  dim2trigluings
  dim2triskeleton
  dim2triui
  dim4eltmovedialog
  dim4trialgebra
  dim4tricreator
  dim4trigluings
  dim4triskeleton
  dim4triui
  eltmovedialog
  facetgraphtab
  gaprunner
  groupwidget
  nanglestructurecreator
  nanglestructureui
  ncompatcanvas
  ncontainerui
  nhypersurfacecreator
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
