Regina for Debian:  Squeeze (Debian 6.0)
----------------------------------------

Debian packages for Regina are not distributed from the SourceForge site.

If you use Squeeze (the Debian 6.0 release), you can obtain packages
from the official Regina repository on people.debian.org.

1. Add Regina's location to your list of repositories.

   This only needs to be done once (the first time you install Regina).
   Open the file /etc/apt/sources.list and add the following line:

     deb http://people.debian.org/~bab/regina squeeze/

2. Update the list of available packages.

   This lets your package manager know that a new version of Regina is
   available.  How you do this depends on which package manager you use;
   an example for aptitude users is given below.

     example$ aptitude update

3. Install or upgrade Regina like you would any other package.

   Again how you do this depends on which package manager you use.
   Aptitude users might run the following command:

     example$ aptitude install regina-normal

   At this point your package manager should download and install Regina
   along with any necessary dependencies.

If you are a C++ programmer and you wish to build Regina into your own
projects, you will also want the package "regina-normal-dev".

If you want the MPI-enabled utilities for use on high-performance clusters,
you will want the package "regina-normal-mpi".

You might have noticed that Squeeze already ships with Regina as part of its
CD/DVD set.  However, this is the older version 4.5 (which was current
when Squeeze was frozen).  For a newer version of Regina you should follow
the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 09:08:56 +1000
