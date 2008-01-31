Setting up a sid chroot under debian etch
-----------------------------------------

This file documents what I've done on my debian etch system to set up a
chroot in which I can build and test packages for regina under debian
unstable (otherwise known as sid).

This is *not* in any way meant to be a general set of instructions for
arbitrary people doing arbitrary things in chroots.  Much better general
instructions can be found on the web.  This document is tailored
specifically to my personal setup, and is intended purely as a reminder
to myself in case I get a new machine and need to do this all over again.

This file was created on 31 January 2008.  Since sid is a fast-moving
target, these instructions might need changing as time goes by.


Prior reading
-------------

This file follows on from README-ubuntu.txt; please read that file first.


Host (debian etch) configuration
--------------------------------

- Same as for the ubuntu chroots; see README-ubuntu.txt for details

- Prepare the sid partition to mount as /srv/chroot/sid


Guest configuration
-------------------

- Bootstrap the system:

  prompt# debootstrap sid /srv/chroot/sid http://ftp.au.debian.org/debian/

- Create guest home directory:

  prompt# mkdir /srv/chroot/sid/home/bab
  prompt# chown bab.bab /srv/chroot/sid/home/bab

- Set up schroot entry:

  [sid]
  type=directory
  description=Debian sid (unstable)
  location=/srv/chroot/sid
  priority=3
  users=bab
  root-users=root
  run-setup-scripts=true
  run-exec-scripts=true

- Install basic KDE system:

  prompt# schroot -c sid aptitude install kdebase kdelibs less vim zsh

- Stop host processes that conflict with the guest install:

  This presumably depends on the current state of debian sid; on the
  31 January install it was only the dbus process that needed stopping.

- Install packages necessary for building regina:

  prompt# schroot -c sid aptitude install
          build-essential devscripts fakeroot lintian
          automake1.9 debhelper doxygen kdelibs4-dev libboost-python-dev
          libcppunit-dev libgmp3-dev libmpich1.0-dev libpopt-dev
          libxml2-dev zlib1g-dev
  prompt# schroot -c sid aptitude clean

- Install other packages useful for running and testing regina:

  prompt# schroot -c sid aptitude install gap graphviz kig mpich-bin

- Set up X authorisation:

  bab@host$ schroot -c sid -- ln -s ../master/bab/.Xauthority .Xauthority

- Start a chrooted session using the inner-session script in this directory:

  bab@host$ schroot -c sid .../path/to/inner-session

