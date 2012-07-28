# Engine

SET ( FILES dim4tricreator dim4trigluings dim4triui eltmovedialog gaprunner nanglestructurecreator nanglestructureui ncompatcanvas ncontainerui nnormalsurfacecreator nnormalsurfaceui npdfui nscriptui nsurfacecompatui nsurfacecoordinateui nsurfacefiltercomb nsurfacefiltercreator nsurfacefilterprop nsurfacematchingui nsurfacesummaryui ntextui ntrialgebra ntriangulationcreator ntriangulationui ntricomposition ntrigluings ntriskeleton ntrisnappea ntrisurfaces skeletonwindow snappeacomponents dim4trialgebra)

SET ( MOC_FILES dim4trigluings.h dim4triui.h nanglestructureui.h gaprunner.h eltmovedialog.h ncompatcanvas.h nnormalsurfaceui.h nscriptui.h nsurfacecompatui.h nsurfacecoordinateui.h nsurfacefiltercomb.h nsurfacefilterprop.h nsurfacematchingui.h nsurfacesummaryui.h ntextui.h ntrialgebra.h ntriangulationui.h ntricomposition.h ntrigluings.h ntriskeleton.h ntrisnappea.h ntrisurfaces.h skeletonwindow.h snappeacomponents.h dim4trialgebra.h)
FOREACH( MOC ${MOC_FILES} )
  SET ( MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/${MOC} )
ENDFOREACH(MOC)
SET ( MOC_HEADERS ${MOC_HEADERS} PARENT_SCOPE)

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} packettypes/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
