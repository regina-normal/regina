
Regina - A normal surface theory calculator

BINARY DISTRIBUTION:

    - As Regina has become more complex and provides more components, it
      has become fussier about where it is installed.

      The binary distribution is designed to be installed immediately
      beneath /usr/local/.  If you wish to install Regina elsewhere (such
      as beneath your home directory), you will probably need to rebuild
      it from source.  It's not very hard; it just takes a while. :)
      See below for details.

    - The binary archive should be unpacked in the root directory.  Each
      file in the archive already has /usr/local prepended to its path.

    - Make sure your BTools jar (and your Jython and JavaHelp jars if you
      have them) are in directory /usr/local/share/regina/jar/.  Otherwise
      you will need to specify their locations using environment variables
      or a configuration file (see the reference manual for details).

    - To start Regina, run /usr/local/bin/regina.

    - Windows users:
        - If you wish to run /usr/local/bin/regina, you will need to do it
          from within a Cygwin bash shell.
        - Alternatively you can run "regina.bat" from your usual windows
          environment.  You will need to edit this batch file before you
          first run it; the batch file itself contains instructions for
          how to do this.

SOURCE DISTRIBUTION:

    - Regina has been reorganised to use the standard "./configure",
      "make" build procedure.  See INSTALL.txt for details.

    - For the impatient, the short summary for building Regina is as
      follows.  INSTALL.txt contains details for customising this process.
        prompt$ ./configure    (determines features of your system)
        prompt$ make           (builds Regina)
        prompt$ make install   (installs Regina)

    - Note that Regina can be run directly out of the source tree (once
      it has been built); there is no need to actually install it if you
      don't want to.  Simply run "./regina" from the top-level source
      directory (the directory containing this file).

Ben Burton <benb@acm.org>
http://regina.sourceforge.net/

