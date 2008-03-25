Regina Packaging Directory
--------------------------

This directory subtree contains helper files for packaging Regina for
various GNU/Linux distributions and related systems.

Only the package maintainer(s) for Regina should need to use these files.

Move into the parent directory and type "make help" for details on
building Regina for yourself.


Subdirectories include:

Packaging and Uploads
---------------------

- debian/ contains packaging directories for debian-based distributions,
  such as Debian and Ubuntu.

- rpmspec/ contains packaging spec files for RPM-based distributions, such
  as Fedora, Mandriva and SuSE.

- finkinfo/ contains packaging info files for Fink on MacOS X.

- readmes/ contains README files that are uploaded to the Regina download
  site (such as package installation instructions for debian, ubuntu and
  fink).  Many are text versions of web pages in regina/www/install/.

Installation of GNU/Linux Distributions
---------------------------------------

- chroots/ contains documentation for installing many different GNU/Linux
  distributions into chroots on a single master machine.  These notes
  are specific to the developers' hardware and setup, and are not intended
  for general use.

- standalone/ contains documentation for installing different GNU/Linux
  distributions into their own partitions for separate booting.  Again
  these notes are specific to the developers' hardware and setup.

