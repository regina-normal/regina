--------
Contents
--------

1. Regina Binary Distribution
2. Regina Source Distribution

-----------------------------
1. Regina Binary Distribution
-----------------------------

To start Regina, run the script /usr/local/bin/regina.

Regina requires various external libraries to be present at runtime.
A full list of requirements can be found on the Regina website
(http://regina.sourceforge.net/).

As Regina provides more and more components, it has become fussier
about where it is installed.  The installation directories are specified
at compile time (as options to ./configure; see below for details).

This means that if you simply download the binary distribution and
then move the files out of /usr/local/ and into some other directory,
Regina will probably not work correctly.  If you wish to use some other
installation directory, you should rebuild Regina from source.

Notes for Windows users:

  - You will need to run this script from within a cygwin bash shell,
    though you may use the batch file /usr/local/bin/regina.bat instead
    to run Regina directly from the Windows desktop.  This batch file
    must be hand-edited before it can be used (the external Windows
    environment cannot guess where you have installed cygwin); the batch
    file itself contains instructions for how to do this.

  - Unlike some previous versions of Regina, this release requires libgmp
    to be installed as a shared library.  If you experience gmp-related
    errors when starting Regina, try downloading the latest gmp-cygwin
    libraries from the Regina download page.

Currently no uninstall utility is provided with the binary
distributions, since this is generally taken care of by your particular
operating system's package management system.  Bug your distribution to
make a package for Regina like Debian does. :) 

Meanwhile, you can look at the contents of the downloaded binary archive
to see which files were installed.  After removing directories
/usr/local/include/regina, /usr/local/lib/regina and
/usr/local/share/regina, there will be very few files left.

-----------------------------
2. Regina Source Distribution
-----------------------------

Regina has been reorganised to use the standard "./configure", "make"
build procedure.  See INSTALL.txt for details.

For the impatient, the short summary is as follows.  INSTALL.txt contains
details for customising this process.

    prompt$ ./configure    (determines features of your system)
    prompt$ make           (builds Regina)
    prompt$ make install   (installs Regina)

Note that Regina can be run directly out of the source tree (once
it has been built); there is no need to actually install it if you
don't want to.  Simply run "./regina" from the top-level source
directory (the directory containing this file).

Regina requires various external libraries to be present during compilation.
A full list of requirements can be found on the Regina website
(http://regina.sourceforge.net/).

The initial "./configure" phase will check which of these libraries are
currently installed on your system, and will decide which Regina components
can be built.  This list will be output immediately before the individual
Makefiles are created, as illustrated below.  You should check this list
to ensure everything you want will be built.  If a component cannot be
built, a reason will be given earlier in the "./configure" output.

    prompt$ ./configure
    ...
    ... (lots of tests)
    ...
    configure: WARNING: No suitable Java compiler could be found; the Java
    user interface will not be built.
    ...
    ... (more tests)
    ...
    configure: WARNING: The JNI headers are unavailable; the JNI calculation
    engine module will not be built.
    ...
    ... (more tests)
    ...
    checking whether to build the calculation engine library... yes
    checking whether to build the JNI calculation engine module... no
    checking whether to build the Java user interface... no
    checking whether to build command-line utilities... yes
    checking whether to build the test suite... yes
    checking whether to build calculation engine docs... yes
    checking whether to build Java user interface docs... no
    checking whether to build the reference manual... yes
    checking whether to build man pages... yes
    checking whether to build the documentation jar... no
    configure: creating ./config.status
    config.status: creating Makefile
    config.status: creating admin/Makefile
    ...
    ... (lots more Makefiles)
    ...
    prompt$

Ben Burton <bab@debian.org>
http://regina.sourceforge.net/

