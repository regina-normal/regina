
Regina - A normal surface theory calculator

BINARY DISTRIBUTION:

    - To run, type "./regina".

    - Windows users:
        - If you wish to run "./regina", you will need to do it from
          within a Cygwin bash shell.
        - Alternatively you can run "regina.bat" from your usual windows
          environment.  You will need to edit this batch file before you
          first run it; the batch file itself contains instructions for
          how to do this.

SOURCE DISTRIBUTION:

    - Before compiling, copy one of the template Makefile.options.* files
      to Makefile.options and edit it to reflect your system.
    - To compile so you have a basic working runtime, type "make binjni".
    - To build the documentation, type "make bindocs".  The documentation
      will be generated and placed in docs/.  A complete documentation
      jar will also be placed in bin/.
    - For other make options, type "make help".

    Be sure to examine Makefile.options to ensure that the configuration
    matches your system.

Ben Burton <benb@acm.org>
http://regina.sourceforge.net/

