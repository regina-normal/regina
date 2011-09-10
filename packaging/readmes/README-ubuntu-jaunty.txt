Regina for Ubuntu:  Jaunty (Ubuntu 9.04)
----------------------------------------

Ubuntu packages for Regina are not distributed from the SourceForge site.

Instead, users of Jaunty Jackalope (the Ubuntu 9.04 release) may obtain
packages from the repository on people.debian.org.  These packages are
available for the i386 and amd64 architectures.  To install them:

1. Add Regina's location to your list of repositories.

   This only needs to be done once (the first time you install Regina).
   Open the Synaptic package manager, select Settings->Repositories,
   click on the tab for third-party software and add a new repository.
   Synaptic will ask you for an APT line, which is:

     deb http://people.debian.org/~bab/regina/ jaunty main

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

You might have noticed that Jaunty Jackalope already offers Regina as part
of its universe section.  However, this is the older version 4.5.1 (which
was current when Jaunty Jackalope was released).  For a newer version of
Regina you should follow the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 09:08:56 +1000
