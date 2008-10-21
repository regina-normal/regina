Regina for Ubuntu:  Hardy (Ubuntu 8.04 LTS)
-------------------------------------------

Ubuntu packages for Regina are not distributed from the SourceForge site.

Instead, users of Hardy Heron (the Ubuntu 8.04 LTS release) may obtain
packages from the repository on people.debian.org.  These packages are
available for the i386 and amd64 architectures.  To install them:

1. Add Regina's location to your list of repositories.

   This only needs to be done once (the first time you install Regina).
   Open the Synaptic package manager, select Settings->Repositories,
   click on the tab for third-party software and add a new repository.
   Synaptic will ask you for an APT line, which is:

     deb http://people.debian.org/~bab/regina/ hardy main

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

Regina is split into several packages for Ubuntu.  To start with you
will probably just want the "regina-normal" package.  If you are doing
Python scripting and/or compiling Regina's mathematical core into your
own code, you may want the "regina-normal-dev" and "regina-normal-doc"
packages also.  The "regina-normal-mpi" package is available for users
running long census calculations on clusters.

You might have noticed that Hardy Heron already offers Regina as part of
its universe section.  However, this is the older version 4.4 (which
was current when Hardy Heron was released).  For a newer version of
Regina you should follow the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sat, 17 May 2008 11:39:33 +1000
