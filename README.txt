Regina Source Distribution
--------------------------

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

Ben Burton <benb@acm.org>
http://regina.sourceforge.net/

