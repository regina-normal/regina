##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = $(all_includes)

noinst_LTLIBRARIES = libpackettypes.la

noinst_HEADERS = \
	nanglestructurecreator.h \
	nanglestructureitem.h \
	nanglestructureui.h \
	ncontainerui.h \
	nfacegluingdialog.h \
	nfacegluingitems.h \
	nscriptui.h \
	nscriptvaritems.h \
	nsurfacefiltercomb.h \
	nsurfacefiltercreator.h \
	nsurfacefilterprop.h \
	ntextui.h \
	ntrialgebra.h \
	ntriangulationui.h \
	ntricomposition.h \
	ntrigluings.h \
	ntriskeleton.h \
	ntrisurfaces.h

libpackettypes_la_SOURCES = \
	nanglestructurecreator.cpp \
	nanglestructureitem.cpp \
	nanglestructureui.cpp \
	ncontainerui.cpp \
	nfacegluingdialog.cpp \
	nfacegluingitems.cpp \
	nscriptui.cpp \
	nscriptvaritems.cpp \
	nsurfacefiltercomb.cpp \
	nsurfacefiltercreator.cpp \
	nsurfacefilterprop.cpp \
	ntextui.cpp \
	ntrialgebra.cpp \
	ntriangulationui.cpp \
	ntricomposition.cpp \
	ntrigluings.cpp \
	ntriskeleton.cpp \
	ntrisurfaces.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

