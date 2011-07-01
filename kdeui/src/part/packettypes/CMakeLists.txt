# Engine

SET ( FILES eltmovedialog gaprunner nanglestructurecreator nanglestructureitem nanglestructureui ncompatcanvas ncontainerui nfacegluingdialog nfacegluingitems nnormalsurfacecreator nnormalsurfaceui npdfui nscriptui nscriptvaritems nsurfacecompatui nsurfacecoordinateitem nsurfacecoordinateui nsurfacefiltercomb nsurfacefiltercreator nsurfacefilterprop nsurfacematchingitem nsurfacematchingui nsurfacesummaryui ntextui ntrialgebra ntriangulationcreator ntriangulationui ntricomposition ntrigluings ntriskeleton ntrisnappea ntrisurfaces skeletonwindow snappeacomponents )

SET ( MOC_FILES nanglestructureui.h gaprunner.h eltmovedialog.h ncompatcanvas.h nfacegluingdialog.h nfacegluingitems.h nnormalsurfaceui.h npdfui.h nscriptui.h nsurfacecompatui.h nsurfacecoordinateui.h nsurfacefiltercomb.h nsurfacefilterprop.h nsurfacematchingui.h nsurfacesummaryui.h ntextui.h ntrialgebra.h ntriangulationui.h ntricomposition.h ntrigluings.h ntriskeleton.h)
FOREACH( MOC ${MOC_FILES} )
  SET ( MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/${MOC} )
ENDFOREACH(MOC)
SET ( MOC_HEADERS ${MOC_HEADERS} PARENT_SCOPE)

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} packettypes/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
