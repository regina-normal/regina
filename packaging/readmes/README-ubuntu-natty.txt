Regina for Ubuntu:  Natty (Ubuntu 11.04)
----------------------------------------

If you use Natty Narwhal (the Ubuntu 11.04 release), you can obtain
packages from the official Regina repository on people.debian.org.

See http://regina.sourceforge.net/www/install/ubuntu-natty.html
for screenshots of each step.

1. Add Regina's location to your list of repositories.
   This only needs to be done once (the first time you install Regina).

   Open the Ubuntu Software Centre and select Edit->Software Sources.

   a) Under "Other Software", click the "Add..." button.  Ubuntu will
      ask you for an APT line, which is:

        deb http://people.debian.org/~bab/regina natty/

   b) Under "Authentication", import Regina's signing key.  These keys
      help keep your machine secure, and Ubuntu may refuse to install
      Regina without one.  You can download the key from:

        http://people.debian.org/~bab/regina-key.txt

      When you import this key file, it should show up in the list of
      trusted software providers as "Ben Burton <bab@debian.org>".
	  After the import, you can delete regina-key.txt if you like.

   c) Under "Ubuntu Software", enable the Community-Maintained Open Source
      Software (universe) if this has not been done before.

2. Install Regina like you would any other package.

   In the Ubuntu Software Centre, just search for the package
   "regina-normal" and press Install.

If you are a C++ programmer and you wish to build Regina into your own
projects, you should also install the package "regina-normal-dev".

If you want the MPI-enabled utilities for use on high-performance clusters,
you should install the package "regina-normal-mpi".

You might notice that Natty Narwhal already offers Regina as part of its
universe section.  However, this is the older version 4.6.  For the latest
version of Regina, you should follow the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 09:08:56 +1000
