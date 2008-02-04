Setting up mandriva chroots under debian etch (amd64)
-----------------------------------------------------

This file documents what I've done on my debian etch system to set up
i586 and x86_64 chroots in which I can build and test mandriva packages
for regina.

This is *not* in any way meant to be a general set of instructions for
arbitrary people doing arbitrary things in chroots.  Much better general
instructions can be found on the web.  This document is tailored
specifically to my personal setup, and is intended purely as a reminder
to myself in case I get a new machine and need to do this all over again.


System layout
-------------

- Unless otherwise specified, all work is done under debian etch
  (the "host debian system").

- A standalone mandriva installation (the "host mandriva system") is
  installed on its own separate partition (currently 4G and 58% full).
  This is mounted within the host debian system in /srv/chroot/mdvhost.

  The host mandriva system is used purely for calling urpmi to bootstrap
  the various mandriva chroots.  At the time of writing it runs mandriva
  2008.0 (x86_64) but this may be upgraded from time to time.

- /srv/chroot/mandriva is a directory on the host debian system containing
  one subdirectory for each final mandriva chroot (the "guest mandriva
  systems").  This is mounted from a separate partition (currently 18G,
  and with plenty of room to spare).


Host mandriva system configuration
----------------------------------

- Boot the host mandriva system and ensure that urpmi is installed.

- Create a /chroot directory in which the guest mandriva systems will be
  mounted.


Host debian system configuration (debian etch)
----------------------------------------------

- Follow the steps described in README-init-schroot.txt to
  set up schroot on the host debian system.

- Mount the guest mandriva partition as /srv/chroot/mandriva, and mount
  the host mandriva partition as /srv/chroot/mdvhost:

  (from /etc/fstab:)
  LABEL=/mandriva /srv/chroot/mandriva ext3 defaults,errors=remount-ro 0 0
  LABEL=/mdvhost /srv/chroot/mdvhost ext3 defaults,errors=remount-ro 0 0

- Bind-mount the guest mandriva partition inside the host mandriva
  partition also:

  (from /etc/fstab:)
  /srv/chroot/mandriva /srv/chroot/mdvhost/chroot none rw,bind 0 0

- Add a schroot entry for the mandriva host, which will only be used as root:

  [mdvhost]
  type=directory
  description=Mandriva chroot host
  location=/srv/chroot/mdvhost
  priority=3
  root-users=root
  run-setup-scripts=true
  run-exec-scripts=true


Guest mandriva system configuration
-----------------------------------

- Enter the mandriva host in order to bootstrap the guest system:

  debian# schroot -c mdvhost

- Add online sources for the guest system:

  mdvhost# urpmi.addmedia --interactive --distrib
           --urpmi-root /chroot/2007.0-x86_64
           http://ftp.riken.go.jp/Linux/mandrake/official/2007.0/x86_64

  The --interactive option asks you to approve each source.  Only
  approve the release sources (main, contrib, etc).  Do not approve the
  updates (which are usually enabled by default).

- Install packages for a base graphical system:

  mdvhost# urpmi --urpmi-root /chroot/2007.0-x86_64
           basesystem urpmi zsh task-kde

- Install packages for building regina:

  mdvhost# urpmi --urpmi-root /chroot/2007.0-x86_64
           rpm-build rpmlint boost-devel cppunit-devel doxygen gmp-devel
           kdelibs-devel libxml2-devel popt-devel python-devel zlib1-devel

- Install other packages for running regina and supporting the chroot:

  mdvhost# urpmi --urpmi-root /chroot/2007.0-x86_64
           graphviz kdeedu x11-server-xnest x11-server-xephyr

- Add a schroot entry in the debian host system:

  [mdv2007.0-x86_64]
  type=directory
  description=Mandriva 2007.0 (x86_64)
  location=/srv/chroot/mandriva/2007.0-x86_64
  priority=3
  users=bab
  root-users=root
  run-setup-scripts=true
  run-exec-scripts=true

  (for x86_64 chroots:)
  aliases=mdv2007.0

  (for i586 chroots:)
  personality=linux32

- Symlink /bin/zsh to /usr/bin/zsh:

  debian# schroot -c mdv2007.0-x86_64 -- ln -s /bin/zsh /usr/bin/zsh

- Manually add an ordinary user:

  debian# schroot -c mdv2007.0-x86_64
  mdvguest# groupadd -g 1000 bab
  mdvguest# useradd -u 1000 -g 1000 bab

- Follow the final steps described in README-final-guestuser.txt to
  start a chrooted session.

