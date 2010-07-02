##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@ $(all_includes) -I$(top_srcdir)/kdeui/src/common

noinst_LTLIBRARIES = libpackettypes.la

noinst_HEADERS = \
	dim4facetgluingdialog.h \
	dim4facetgluingitems.h \
	dim4tricreator.h \
	dim4trigluings.h \
	dim4triui.h \
	eltmovedialog.h \
	gaprunner.h \
	nanglestructurecreator.h \
	nanglestructureitem.h \
	nanglestructureui.h \
	ncompatcanvas.h \
	ncontainerui.h \
	nfacegluingdialog.h \
	nfacegluingitems.h \
	nnormalsurfacecreator.h \
	nnormalsurfaceui.h \
	npdfui.h \
	nscriptui.h \
	nscriptvaritems.h \
	nsurfacecompatui.h \
	nsurfacecoordinateitem.h \
	nsurfacecoordinateui.h \
	nsurfacefiltercomb.h \
	nsurfacefiltercreator.h \
	nsurfacefilterprop.h \
	nsurfacematchingitem.h \
	nsurfacematchingui.h \
	nsurfacesummaryui.h \
	ntextui.h \
	ntrialgebra.h \
	ntriangulationcreator.h \
	ntriangulationui.h \
	ntricomposition.h \
	ntrigluings.h \
	ntriskeleton.h \
	ntrisnappea.h \
	ntrisurfaces.h \
	skeletonwindow.h \
	snappeacomponents.h

libpackettypes_la_SOURCES = \
	dim4facetgluingdialog.cpp \
	dim4facetgluingitems.cpp \
	dim4tricreator.cpp \
	dim4trigluings.cpp \
	dim4triui.cpp \
	eltmovedialog.cpp \
	gaprunner.cpp \
	nanglestructurecreator.cpp \
	nanglestructureitem.cpp \
	nanglestructureui.cpp \
	ncompatcanvas.cpp \
	ncontainerui.cpp \
	nfacegluingdialog.cpp \
	nfacegluingitems.cpp \
	nnormalsurfacecreator.cpp \
	nnormalsurfaceui.cpp \
	npdfui.cpp \
	nscriptui.cpp \
	nscriptvaritems.cpp \
	nsurfacecompatui.cpp \
	nsurfacecoordinateitem.cpp \
	nsurfacecoordinateui.cpp \
	nsurfacefiltercomb.cpp \
	nsurfacefiltercreator.cpp \
	nsurfacefilterprop.cpp \
	nsurfacematchingitem.cpp \
	nsurfacematchingui.cpp \
	nsurfacesummaryui.cpp \
	ntextui.cpp \
	ntrialgebra.cpp \
	ntriangulationcreator.cpp \
	ntriangulationui.cpp \
	ntricomposition.cpp \
	ntrigluings.cpp \
	ntriskeleton.cpp \
	ntrisnappea.cpp \
	ntrisurfaces.cpp \
	skeletonwindow.cpp \
	snappeacomponents.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

