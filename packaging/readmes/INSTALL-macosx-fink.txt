Regina for MacOS X:  Fink Packages
----------------------------------

The good news: Regina runs on MacOS X, and the user experience is *much*
cleaner and more natural than the old (pre-2011) versions.  See the
screenshots on regina.sourceforge.net, all of which were taken on my Mac.

The bad news: You still need Fink.  Regina is written for GNU/Linux, and
Fink makes the many compile-time and run-time dependencies available for
Mac users.  We are still a long way from a drag-and-drop Mac application.

To install Regina, you will need:

1. MacOS X Leopard (10.5) or Snow Leopard (10.6):

   Fink has not yet been fully ported to Lion (10.7), which means neither
   has Regina.  The Fink people are working on it, and hopefully Regina
   will be available on Lion soon.

2. The Apple XCode/Developer Tools:

   This gives you a C++ compiler and other tools.  You should find the
   Developer Tools on your MacOS X install discs, but you can also
   download them from Apple for free.

3. A source-based Fink install, including the "unstable" tree:

   You need to install Fink as a source distribution, not a binary
   distribution.  To install Fink, see:

     http://www.finkproject.org/download/srcdist.php

   You also need to enable the "unstable" tree.  Run "fink configure",
   and when it asks you about the "unstable" tree, say yes.

4. The Fink info file for Regina:

   This tells Fink how to build and install Regina.  Download it from:

     http://regina.sourceforge.net/install/fink/regina-normal.info

   Copy it into /sw/fink/dists/local/main/finkinfo/ so that Fink can find it:

     example$ sudo cp regina-normal.info /sw/fink/dists/local/main/finkinfo


Now you can build and install Regina:

5. Make sure that Fink can see the new version of Regina:

     example$ fink list regina-normal
     Information about 10252 packages read in 1 seconds.
          regina-normal    4.90-0.1     Computational 3-manifold ...
          regina-normal-d  4.4-3        Development files for ...
          regina-normal4.  4.3.1-2      Shared libraries for ...
          ... (lots more output)

   You will see older versions of Regina as well (these are coming from Fink),
   but the "regina-normal" package at the top of the list should show the
   new version (4.90-0.1).  If you only see old versions, check that you
   copied regina-normal.info to the correct location in step 4 above.

6. Build and install Regina as you would any other Fink package:

   example$ fink install regina-normal

7. Be patient!

   Fink will download and build all of the components that Regina needs,
   including the substantial Qt and KDE libraries and all of *their*
   prerequisites.  This may take several hours, and you might want to
   leave the build running overnight.

8. For C++ programmers:

   If you are a C++ programmer and you wish to build Regina into your own
   projects, you will also want the Regina development package:

     example$ fink install regina-normal-dev


To run Regina, open the Finder and browse to /sw/opt/kde4/mac/bin/regina-kde
on your hard drive.  You can drag this app onto your dock if you like.

That's all!  It takes a while to install, but again the end product looks
and feels much better than Regina ever did on the Mac before.

Please mail me at bab@debian.org if you run into difficulties.

For simple fixes for some Mac-related issues that you might encounter, see:

  http://regina.sourceforge.net/docs/troubleshooting.html#trouble-macosx

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 08:21:55 +1000
