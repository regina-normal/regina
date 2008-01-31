Setting up ubuntu chroots under debian etch
-------------------------------------------

This file documents what I've done on my debian etch system to set up
chroots in which I can build and test ubuntu packages for regina.

This is *not* in any way meant to be a general set of instructions for
arbitrary people doing arbitrary things in chroots.  Much better general
instructions can be found on the web.  This document is tailored
specifically to my personal setup, and is intended purely as a reminder
to myself in case I get a new machine and need to do this all over again.


Host (debian etch) configuration
--------------------------------

- Install debootstrap from sid (which has ubuntu hooks)

- Install schroot

- Install hpilp (to avoid later errors in $CHROOT/var/lib/dpkg/statoverride)

- Edit /etc/schroot/setup.d/10mount to mount host /home as guest /home/master,
  to avoid mixing configurations for different versions of software:

  (line 82:)
  -           do_mount "-o rw,bind" "/home"    "${CHROOT_PATH}/home"
  +           do_mount "-o rw,bind" "/home"    "${CHROOT_PATH}/home/master"

- Edit /etc/schroot/setup.d/20network to copy /etc/hosts:

  (line 8-9:)
     cp $VERBOSE /etc/resolv.conf "${CHROOT_PATH}/etc/resolv.conf"
  +  cp $VERBOSE /etc/hosts "${CHROOT_PATH}/etc/hosts"

- Prepare the ubuntu partition to mount as /srv/chroot/ubuntu


Guest configuration
-------------------

- Bootstrap the system:

  (for amd64:)
  prompt# debootstrap gutsy /srv/chroot/ubuntu/gutsy-amd64
          http://archive.ubuntu.com/ubuntu/

  (for i386:)
  prompt# debootstrap --arch i386 gutsy /srv/chroot/ubuntu/gutsy-i386
  http://archive.ubuntu.com/ubuntu/

- Create guest home directory:

  prompt# mkdir /srv/chroot/ubuntu/gutsy-amd64/home/bab
  prompt# chown bab.bab /srv/chroot/ubuntu/gutsy-amd64/home/bab

- Set up schroot entry:

  [gutsy-amd64]
  type=directory
  description=Ubuntu gutsy (amd64)
  location=/srv/chroot/ubuntu/gutsy-amd64
  priority=3
  users=bab
  root-users=root
  run-setup-scripts=true
  run-exec-scripts=true

  (for amd64 chroots:)
  aliases=gutsy

  (for i386 chroots:)
  personality=linux32

- Add universe to /etc/apt/sources.list:

  deb http://archive.ubuntu.com/ubuntu gutsy main universe

  prompt# schroot -c gutsy aptitude update

- Stop host processes that conflict with the guest install:

  prompt# /etc/init.d/acpid stop  (*** This may cause X to restart! ***)

- Install base system:

  prompt# schroot -c gutsy aptitude install
          ubuntu-desktop language-pack-en man xnest xserver-xephyr

- Purge packages that cause problems in chroots:

  (to avoid problems copying the host /etc/resolv.conf:)
  prompt# schroot -c gutsy aptitude purge resolvconf

  (to avoid problems in the guest /var/lib/dpkg/statoverride:)
  prompt# schroot -c gutsy aptitude purge postfix

- Install packages necessary for building regina:

  prompt# schroot -c gutsy aptitude install
          build-essential fakeroot lintian automake1.9 debhelper doxygen
          kdelibs4-dev libboost-python-dev libcppunit-dev libgmp3-dev
          libmpich1.0-dev libpopt-dev libxml2-dev zlib1g-dev
  prompt# schroot -c gutsy aptitude clean

- Install other packages needed when running regina:

  prompt# schroot -c gutsy aptitude install graphviz khelpcenter konqueror

- Allow user to administer the system:

  prompt# schroot -c gutsy visudo

  bab ALL=(ALL) ALL

- Set up X authorisation:

  bab@host$ schroot -c gutsy -- ln -s ../master/bab/.Xauthority .Xauthority

- Start a chrooted session using the inner-session script in this directory:

  bab@host$ schroot -c gutsy .../path/to/inner-session

