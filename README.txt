--------
Contents
--------

0. Requirements
1. Regina Binary Distribution
2. Regina Source Distribution

---------------
0. Requirements
---------------

Regina relies upon a variety of external libraries and other tools.
These are listed below.  They are available with most GNU/Linux
distributions, but the Regina website (http://regina.sourceforge.net/)
contains a list of locations from which they can be downloaded
separately if necessary.

Essential:
    libgmp (the GNU multiprecision arithmetic library), version >= 3
    libxml2 (the GNOME XML library), version >= 2.0.0
    libpopt (the Popt command-line parsing library), version >= 1.3

KDE User Interface (optional):
    kdelibs (the core KDE libraries), version >= 3.1

Python Bindings (optional):
    python (the scripting language), version >= 2.2
    libboost_python (the Boost.Python libraries), verison >= 1.30.0

Documentation (optional, only required at compile-time):
    doxygen (a C++ documentation system), version >= 1.2.2
    docbook-utils (includes docbook2man), version >= 0.6.9

-----------------------------
1. Regina Binary Distribution
-----------------------------

Binary distributions are no longer provided, since they depend so
strongly upon which versions of external libraries are installed and
where they are to be found.

It is recommended instead to install the dependencies described above,
download a source distribution of Regina and build it to work with your
system.  Otherwise you can bug your distribution to provide packages for
Regina like Debian does. ;-)

-----------------------------
2. Regina Source Distribution
-----------------------------

Regina now uses the standard "./configure", "make"
build procedure.  See INSTALL.txt for details.

For the impatient, the short summary is as follows.  INSTALL.txt contains
details for customising this process.

    prompt$ ./configure    (determines features of your system)
    prompt$ make           (builds Regina)
    prompt$ make install   (installs Regina)

Note that for the KDE user interface to work, you will almost certainly
need to pass --prefix and possibly other options to ./configure so that
files are installed in the correct places.  Run "./configure --help" for
a list of available options.

In particular, Regina must be installed in a place where KDE will find
it, since it provides most of its functionality as a KPart.

The initial "./configure" phase will check which of the required libraries are
currently installed on your system, and will decide which Regina components
can be built.  This list will be output immediately before the individual
Makefiles are created, as illustrated below.  You should check this list
to ensure everything you want will be built.  If a component cannot be
built, a reason will be given earlier in the "./configure" output.

    prompt$ ./configure
    ...
    ... (lots of tests)
    ...
    configure: WARNING: A working boost.python installation is
    unavailable; the Python interface will not be built.
    ...
    ... (more tests)
    ...
    checking whether to build the calculation engine library... yes
    checking whether to build the KDE user interface... yes
    checking whether to build the Python interface... no
    checking whether to build command-line utilities... yes
    checking whether to build the test suite... yes
    checking whether to build calculation engine docs... yes
    checking whether to build man pages... yes
    configure: creating ./config.status
    config.status: creating Makefile
    config.status: creating admin/Makefile
    ...
    ... (lots more Makefiles)
    ...
    prompt$

Ben Burton <bab@debian.org>
http://regina.sourceforge.net/

