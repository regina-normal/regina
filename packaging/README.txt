Regina Packaging Directory
--------------------------

This directory tree contains helper files for packaging Regina for
different GNU/Linux distributions and other systems.

Only the package maintainer(s) for Regina should need to use these files.


Subdirectories include:

Packaging and Uploads
---------------------

- debian/ contains packaging for debian-based distributions,
  such as Debian and Ubuntu.

- rpmspec/ contains packaging for RPM-based distributions, such
  as Fedora, Mandriva and SuSE.

- finkinfo/ contains packaging for MacOSX/Fink.

- readmes/ contains README files that are uploaded to the Regina download
  site (such as package installation instructions for debian, ubuntu and
  fink).  Many are text versions of the web pages in regina/www/install/.

Installation of GNU/Linux Distributions
---------------------------------------

- chroots/ contains documentation for installing many different GNU/Linux
  distributions into chroots on a single master machine.  These notes
  are specific to the developers' hardware and setup, and are not intended
  for general use.

- standalone/ contains documentation for installing different GNU/Linux
  distributions into their own partitions for separate booting.  Again
  these notes are specific to the developers' hardware and setup.


If you are interested in helping package Regina for one or more
distributions, please contact the author at the address below.

 -- Ben Burton <bab@debian.org>, Thu,  8 Sep 2011 14:15:23 +1000
