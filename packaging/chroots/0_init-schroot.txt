Host debian system:  Initial schroot setup
------------------------------------------

This details the initial steps for setting up schroot on the host debian
system.  These steps need to be performed before any chroots are created.

Please see the distribution-specific README files (such as
README-ubuntu.txt or README-mandriva.txt) for full instructions on
setting up a chroot for building and testing regina.


Assumptions
-----------

- The host system is running debian etch (amd64).


Host schroot configuration
--------------------------

- Install schroot.

- Edit /etc/schroot/setup.d/10mount to mount host /home as guest /home/master,
  to avoid mixing configurations for different versions of software:

  (line 82:)
  -           do_mount "-o rw,bind" "/home"    "${CHROOT_PATH}/home"
  +           do_mount "-o rw,bind" "/home"    "${CHROOT_PATH}/home/master"

- Edit /etc/schroot/setup.d/20network to copy /etc/hosts:

  (line 8-9:)
     cp $VERBOSE /etc/resolv.conf "${CHROOT_PATH}/etc/resolv.conf"
  +  cp $VERBOSE /etc/hosts "${CHROOT_PATH}/etc/hosts"

- Edit /etc/schroot/setup.d/30passwd so that it only copies password
  files on debian-based systems:

  (line 1:)
   #!/bin/sh
   
  +# Only do this on debian/ubuntu systems.
  +# For RPM-based systems we will just add users and groups manually.
  +if [ ! -e ${CHROOT_PATH}/var/lib/dpkg/status ]; then
  +  exit
  +fi
  +
   if [ "$AUTH_VERBOSITY" = "verbose" ]; then

