# Engine

SET ( FILES dim2tricreator dim2trigluings dim2triskeleton dim2triui eltmovedialog gaprunner nanglestructurecreator nanglestructureui ncompatcanvas ncontainerui nnormalsurfacecreator nnormalsurfaceui npdfui nscriptui nsurfacecompatui nsurfacecoordinateui nsurfacefiltercomb nsurfacefiltercreator nsurfacefilterprop nsurfacematchingui nsurfacesummaryui ntextui ntrialgebra ntriangulationcreator ntriangulationui ntricomposition ntrigluings ntriskeleton ntrisurfaces skeletonwindow )

SET ( MOC_FILES dim2trigluings.h dim2triskeleton.h dim2triui.h nanglestructureui.h gaprunner.h eltmovedialog.h ncompatcanvas.h nnormalsurfaceui.h nscriptui.h nsurfacecompatui.h nsurfacecoordinateui.h nsurfacefiltercomb.h nsurfacefilterprop.h nsurfacematchingui.h nsurfacesummaryui.h ntextui.h ntrialgebra.h ntriangulationui.h ntricomposition.h ntrigluings.h ntriskeleton.h ntrisurfaces.h skeletonwindow.h )

IF (NOT EXCLUDE_SNAPPEA)
  SET (FILES ${FILES} ntrisnappea snappeacomponents)
  SET (MOC_FILES ${MOC_FILES} ntrisnappea.h snappeacomponents.h)
ENDIF (NOT EXCLUDE_SNAPPEA)

FOREACH( MOC ${MOC_FILES} )
  SET ( MOC_HEADERS ${MOC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/${MOC} )
ENDFOREACH(MOC)
SET ( MOC_HEADERS ${MOC_HEADERS} PARENT_SCOPE)

FOREACH( FILE ${FILES} )
  SET( SOURCES ${SOURCES} packettypes/${FILE})
ENDFOREACH(FILE)
SET( SOURCES ${SOURCES} PARENT_SCOPE)
