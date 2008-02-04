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

- Follow the steps described in README-init-schroot.txt to
  set up schroot on the host debian system.

- Install debootstrap.

- Mount the sid partition as /srv/chroot/sid:

  (from /etc/fstab:)
  LABEL=/sid /srv/chroot/sid ext3 defaults,errors=remount-ro 0 0


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

- Install basic graphical system:

  prompt# schroot -c sid aptitude install
          less vim zsh kdelibs kdebase xnest xserver-xephyr

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

- Follow the final steps described in README-final-guestuser.txt to
  start a chrooted session.

