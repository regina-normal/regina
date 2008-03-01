Guest chroot configuration:  Final steps
----------------------------------------

This details the final steps for setting up the guest user's account in
a chroot.  These steps are common to most (if not all) chroot setups.

Please see the distribution-specific README files (such as
README-ubuntu.txt or README-mandriva.txt) for full instructions on
setting up a chroot for building and testing regina.


Assumptions
-----------

- The host system is running debian etch.  In the examples below the
  guest system is ubuntu gutsy (amd64), whose schroot name is "gutsy".


Final guest configuration
-------------------------

- Symlink important files in the guest user's home directory, including
  X authorisation and the inner-session script:

  bab@debian$ schroot -c gutsy --
              ln -s ../master/bab/.Xauthority .Xauthority
  bab@debian$ schroot -c gutsy --
              ln -s ../master/bab master
  bab@debian$ schroot -c gutsy --
              ln -s ../master/bab/svn/regina/regina/packaging/chroots/inner-session inner-session

  Note that the inner-session script is symlinked from the regina source
  tree, so its location might vary.

- Copy important dotfiles into the guest user's home directory:

  bab@debian$ cp ~/.vimrc ~/.zprofile ~/.zshrc
              /srv/chroot/ubuntu/gutsy-amd64/home/bab

- Edit the guest user's zsh prompt to make it clear when the user is
  inside the chroot.  For example, in the prompt line "export PS1=...",
  replace "%M" (the machine name) with "%M_gutsy-i386" (or whatever).

- A chrooted session can now be started using the inner-session script:

  bab@debian$ schroot -c gutsy ./inner-session
