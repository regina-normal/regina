## Makefile.am for regina

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = regina regina_client

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
regina_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
regina_LDADD   = $(LIB_KFILE)

# which sources should be compiled for regina
regina_SOURCES = main.cpp regina.cpp reginaview.cpp pref.cpp reginaiface.skel

# these are the headers for your project
noinst_HEADERS = regina.h reginaview.h pref.h

# client stuff
regina_client_LDFLAGS = $(KDE_RPATH) $(all_libraries)
regina_client_LDADD   = $(LIB_KDECORE)
regina_client_SOURCES = regina_client.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

KDE_ICON = regina

# this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Applications
kdelnk_DATA = regina.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/regina
rc_DATA = reginaui.rc
