Regina for Ubuntu:  Lucid (Ubuntu 10.04 LTS)
--------------------------------------------

If you use Lucid Lynx (the Ubuntu 10.04 LTS release), you can obtain
packages from the official Regina repository on people.debian.org.

This means that Ubuntu will automatically fetch dependencies for you, and
your package manager will know when new versions of Regina are released.

See http://regina.sourceforge.net/www/install/ubuntu-lucid.html
for screenshots of each step.

1. Add Regina's location to your list of repositories.  To do this,
   open the Ubuntu Software Centre and select Edit->Software Sources.

   a) Under "Authentication", import Regina's cryptographic signing key.
      This helps keep your machine secure, and Ubuntu may refuse to install
      Regina without it.  You can download the key from:

        http://people.debian.org/~bab/regina-key.txt

      When you import this key file, it should show up in the list of
      trusted software providers as "Ben Burton <bab@debian.org>".
      After the import, you can delete regina-key.txt if you like.

   b) Under "Other Software", click the "Add..." button.  When Ubuntu
      asks you for an APT line, type:

        deb http://people.debian.org/~bab/regina lucid/

   c) Under "Ubuntu Software", enable the Community-Maintained Open Source
      Software (universe) if this has not been done before.

2. Install Regina like you would any other package.  Just search for the
   package "regina-normal" in the Ubuntu Software Centre, and press Install.

If you are a C++ programmer and you wish to build Regina into your own
projects, you should also install the package "regina-normal-dev".

If you want the MPI-enabled utilities for use on high-performance clusters,
you should install the package "regina-normal-mpi".

You might notice that Lucid Lynx already offers Regina as part of its
universe section.  However, this is the older version 4.6.  For the latest
version of Regina, you should follow the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 09:08:56 +1000
