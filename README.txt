--------
Contents
--------

0. Requirements
1. Building Regina from Source
2. Tips and Warnings

---------------
0. Requirements
---------------

Regina relies upon a variety of external libraries and other tools
that are available with most GNU/Linux distributions.  This list of
requirements is kept up-to-date on the Regina website, where you can
also find specific information on how to obtain them for your particular
GNU/Linux distribution:

    http://regina.sourceforge.net/source.html

Note that for each required library, you will need both the library and
its development files.  As an example, for the GNU multiprecision
arithmetic library (libgmp), Debian users should install both the libgmp3
and libgmp3-dev packages, and Fedora Core users should install both the
gmp and gmp-devel packages.

------------------------------
1. Building Regina from Source
------------------------------

The short summary of building Regina is illustrated below.  For glorious
details on how this process can be customised, see the file INSTALL.txt.

If you have difficulty compiling or running Regina, you can check the
troubleshooting page to see if your problem has already been encountered:

    http://regina.sourceforge.net/docs/troubleshooting.html

The steps involved are as follows.

    prompt$ ./configure    (to determine features of your system)
    ...                    (wait a couple of minutes)
    prompt$ make           (to build Regina)
    ...                    (wait an hour or two)
    prompt$ su             (you must be root to install Regina)
    prompt$ make install   (to install Regina)
    ...                    (wait a couple of minutes)
    prompt$                (all done!)

To start Regina once you've installed it, run "regina-kde" (or
alternatively try to locate Regina in your menu of applications).

For some systems this build procedure will need to be customised.  For
this reason it is strongly recommended that you read the tips and warnings
section below.

--------------------
2. Tips and Warnings
--------------------

Regina *must* be installed in the same location as all other KDE
applications, or it will not run properly.  You can tell ./configure
where to install Regina by passing an appropriate --prefix option.
However, in most cases the ./configure script can work this out on its
own.  Generally you should not be passing your own --prefix unless it
is absolutely necessary (e.g., the ./configure script tells you that the
prefix it's currently trying to use looks incorrect).

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
    configure: creating ./config.status
    config.status: creating Makefile
    config.status: creating admin/Makefile
    ...
    ... (lots more Makefiles)
    ...
    prompt$

If the configure script fails to find some tools or libraries that you
know are installed, you can generally help it along by passing
additional arguments to the configure script, e.g.:

    prompt$ ./configure --with-qt-dir=/usr/lib/qt-3.3

Once more, if you run into problems then you might find a solution on the
ever-expanding troubleshooting page:

    http://regina.sourceforge.net/docs/troubleshooting.html

Alternatively you are of course welcome to email either myself or the user
list (regina-user@lists.sourceforge.net) for assistance.

Ben Burton (bab@debian.org)
http://regina.sourceforge.net/

