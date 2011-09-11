Regina for Debian:  Squeeze (Debian 6.0)
----------------------------------------

If you use Squeeze (the Debian 6.0 release), you can obtain packages
from the official Regina repository on people.debian.org:

1. Add Regina's location to your list of repositories.

   This only needs to be done once (the first time you install Regina).
   Open the file /etc/apt/sources.list and add the following line:

     deb http://people.debian.org/~bab/regina squeeze/

2. Update the list of available packages.  At a command prompt, type:

     aptitude update

3. Install or upgrade Regina like you would any other package.  Type:

     aptitude install regina-normal

If you are a C++ programmer and you wish to build Regina into your own
projects, you should also install the package "regina-normal-dev".

If you want the MPI-enabled utilities for use on high-performance clusters,
you should install the package "regina-normal-mpi".

You might notice that Squeeze already ships with Regina as part of its
CD/DVD set.  However, this is the older version 4.6.  For the latest
version of Regina, you should follow the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 09:08:56 +1000
