Regina for Debian:  Squeeze (Debian 6.0)
----------------------------------------

If you use Squeeze (the Debian 6.0 release), you can obtain packages
from the official Regina repository on people.debian.org.

This means that Debian will automatically fetch dependencies for you, and
your package manager will know when new versions of Regina are released.

1. Make Regina's repository a trusted source.  This helps keep your machine
   secure, and avoids messages like "these packages cannot be authenticated".
   To do this, download Regina's cryptographic signing key:
      
        http://people.debian.org/~bab/regina-key.txt

   Then, at a command prompt type:

        sudo apt-key add ~/Downloads/regina-key.txt

   Here you should replace ~/Downloads/regina-key.txt with location of
   the key file.  You can now delete regina-key.txt if you like.

2. Add Regina's location to your list of repositories.  To do this, open
   /etc/apt/sources.list and add the following line:

     deb http://people.debian.org/~bab/regina squeeze/

3. Update the list of available packages.  At a command prompt, type:

     sudo aptitude update

4. Install or upgrade Regina like you would any other package.  Type:

     sudo aptitude install regina-normal

If you are a C++ programmer and you wish to build Regina into your own
projects, you should also install the package "regina-normal-dev".

If you want the MPI-enabled utilities for use on high-performance clusters,
you should install the package "regina-normal-mpi".

You might notice that Squeeze already ships with Regina as part of its
CD/DVD set.  However, this is the older version 4.6.  For the latest
version of Regina, you should follow the steps outlined above.

------------------------------------

Ben Burton <bab@debian.org>, Sun, 11 Sep 2011 09:08:56 +1000
