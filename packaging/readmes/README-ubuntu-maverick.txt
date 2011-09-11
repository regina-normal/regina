Regina for Ubuntu:  Maverick (Ubuntu 10.10)
-------------------------------------------

If you use Maverick Meerkat (the Ubuntu 10.10 release), you can obtain
packages from the official Regina repository on people.debian.org.

This means that Ubuntu will automatically fetch dependencies for you, and
your package manager will know when new versions of Regina are released.

1. Add Regina's location to your list of repositories.

   This only needs to be done once (the first time you install Regina).
   Open the Synaptic package manager, select Settings->Repositories,
   click on the tab for third-party software and add a new repository.
   Synaptic will ask you for an APT line, which is:

     deb http://people.debian.org/~bab/regina natty/

   You should also enable the Community Maintained Universe in your
   list of repositories if this has not been done before.

2. Update the list of available packages.

   This lets your package manager know that a new version of Regina is
   available.  Synaptic users can do this by pressing the Reload button.

3. Install or upgrade Regina like you would any other package.

   The main package to install is called "regina-normal" (though there
   are others; see below).  Synaptic users will find this package in the
   Mathematics (universe) section.

   At this point your package manager should download and install Regina
   along with any necessary dependencies.

If you are a C++ programmer and you wish to build Regina into your own
projects, you will also want the package "regina-normal-dev".

If you want the MPI-enabled utilities for use on high-performance clusters,
you will want the package "regina-normal-mpi".

You might notice that Maverick Meerkat already offers Regina as part of its
universe section.  However, this is the older version 4.6.  For the latest
version of Regina, you should follow the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 09:08:56 +1000
