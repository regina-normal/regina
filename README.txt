
Regina - A normal surface theory calculator

BINARY DISTRIBUTION:

    - Make sure your BTools jar (and your Jython and JavaHelp jars if
      you have them) are in the jar/ directory.  Otherwise you will need
      to specify their locations using environment variables or a
      configuration file (see the reference manual for details).

    - To run, type "./regina".

    - Windows users:
        - If you wish to run "./regina", you will need to do it from
          within a Cygwin bash shell.
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

Ben Burton <benb@acm.org>
http://regina.sourceforge.net/

