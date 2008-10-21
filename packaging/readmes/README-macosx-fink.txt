Regina for MacOS X:  Fink Packages
----------------------------------

Regina now builds and runs under MacOS X, thanks to the Fink project
(http://www.finkproject.org/).

To run Regina, you will need:

1. A working source-based Fink installation.

   Fink can be used as a binary distribution (download pre-built
   packages only) or as a source distribution (automatically build each
   package from sources on your machine).  To run Regina you will need
   the source distribution, since the binary packages for Fink are too
   old and do not include all of the required software.

   You will also need the Apple XCode/Developer Tools (this allows you
   to build Fink packages) and the Apple X11 distribution including both
   the X11User and X11SDK packages (this allows you to build packages
   with graphical user interfaces).

   The Fink Quick Start page at http://www.finkproject.org/download/
   explains how you can get all of this up and running.

   Regina is known to work with the Fink 10.4 tree (it has not been tried
   under Leopard).  Both the stable and unstable branches of Fink are
   supported.

2. The Fink info file for Regina.

   The info file is a short text file that tells Fink how to build and
   install Regina.  You can download it from:

     http://regina.sourceforge.net/install/fink/regina-normal.info

   Copy it into /sw/fink/dists/local/main/finkinfo/ so that Fink can find it:

     example$ sudo cp regina-normal.info /sw/fink/dists/local/main/finkinfo

   Users of the Fink unstable branch can skip this step, since Regina is
   now included in the Fink distribution.  Please note though that new
   versions of Regina might not appear in the Fink archive until a few
   weeks after their release.

You can now build and install Regina as you would any other Fink package:

  example$ fink install regina-normal

Be prepared to wait, since Fink might need to download and build
additional components such as the Boost and KDE libraries.

If you wish to build Regina into your own C++ projects, you will also
need the Regina development package:

  example$ fink install regina-normal-dev

MacOS X support is very new for Regina, so please do mail me at
bab@debian.org if you run into difficulties.  It is hoped that future
releases of Regina can be offered as standalone pre-build Mac
applications (the sort that you simply drag onto the hard drive), but we
shall see how that goes.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 25 Nov 2007 10:28:59 +1100
