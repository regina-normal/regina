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
	eltmovedialog.h \
	gaprunner.h \
	nanglestructurecreator.h \
	nanglestructureitem.h \
	nanglestructureui.h \
	ncontainerui.h \
	nfacegluingdialog.h \
	nfacegluingitems.h \
	nnormalsurfacecreator.h \
	nnormalsurfaceui.h \
	nscriptui.h \
	nscriptvaritems.h \
	nsurfacecoordinateitem.h \
	nsurfacecoordinateui.h \
	nsurfacefiltercomb.h \
	nsurfacefiltercreator.h \
	nsurfacefilterprop.h \
	nsurfacematchingitem.h \
	nsurfacematchingui.h \
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
	eltmovedialog.cpp \
	gaprunner.cpp \
	nanglestructurecreator.cpp \
	nanglestructureitem.cpp \
	nanglestructureui.cpp \
	ncontainerui.cpp \
	nfacegluingdialog.cpp \
	nfacegluingitems.cpp \
	nnormalsurfacecreator.cpp \
	nnormalsurfaceui.cpp \
	nscriptui.cpp \
	nscriptvaritems.cpp \
	nsurfacecoordinateitem.cpp \
	nsurfacecoordinateui.cpp \
	nsurfacefiltercomb.cpp \
	nsurfacefiltercreator.cpp \
	nsurfacefilterprop.cpp \
	nsurfacematchingitem.cpp \
	nsurfacematchingui.cpp \
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

