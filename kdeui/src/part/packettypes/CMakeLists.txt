# Engine

SET ( FILES eltmovedialog gaprunner nanglestructurecreator nanglestructureitem nanglestructureui ncompatcanvas ncontainerui nfacegluingdialog nfacegluingitems nnormalsurfacecreator nnormalsurfaceui npdfui nscriptui nscriptvaritems nsurfacecompatui nsurfacecoordinateitem nsurfacecoordinateui nsurfacefiltercomb nsurfacefiltercreator nsurfacefilterprop nsurfacematchingitem nsurfacematchingui nsurfacesummaryui ntextui ntrialgebra ntriangulationcreator ntriangulationui ntricomposition ntrigluings ntriskeleton ntrisnappea ntrisurfaces skeletonwindow snappeacomponents )

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} packettypes/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
