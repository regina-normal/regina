Regina for Debian:  Lenny (Debian 5.0)
--------------------------------------

Debian packages for Regina are not distributed from the SourceForge site.

Instead, users of Lenny (the forthcoming Debian 5.0 release, which is
frozen at the time of writing) may obtain packages from the repository
on people.debian.org.  These packages are available for the i386 and
amd64 architectures.  To install them:

1. Add Regina's location to your list of repositories.

   This only needs to be done once (the first time you install Regina).
   Open the file /etc/apt/sources.list and add the following line:

     deb http://people.debian.org/~bab/regina/ lenny main

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

Regina is split into several packages for Debian.  To start with you
will probably just want the "regina-normal" package.  If you are doing
Python scripting and/or compiling Regina's mathematical core into your
own code, you may want the "regina-normal-dev" and "regina-normal-doc"
packages also.  The "regina-normal-mpi" package is available for users
running long census calculations on clusters.

You might have noticed that Lenny already ships with Regina as part of its
CD/DVD set.  However, this is the older version 4.5 (which was current
when Lenny was frozen).  For a newer version of Regina you should follow
the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sat, 16 May 2009 14:36:16 +1000
