Regina for Debian:  Sid (Unstable)
----------------------------------

Debian packages for Regina are not distributed from the SourceForge site.

Instead, users of Sid (the Debian unstable release) may obtain packages
from the usual Debian servers.  Debian Sid always ships with the latest
version of Regina, which means that you can install Regina just like you
would any other package:

  example$ aptitude install regina-normal

Regina is split into several packages for Debian.  To start with you
will probably just want the "regina-normal" package.  If you are doing
Python scripting and/or compiling Regina's mathematical core into your
own code, you may want the "regina-normal-dev" and "regina-normal-doc"
packages also.  The "regina-normal-mpi" package is available for users
running long census calculations on clusters.

Please note that new versions of Regina might not appear on the Debian
servers until a few days after their release.

------------------------------------

Ben Burton <bab@debian.org>, Sat, 17 May 2008 11:39:33 +1000
