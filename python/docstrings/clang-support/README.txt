Clang python bindings
---------------------

This directory contains the clang python bindings, taken directly from the
LLVM source code.  These are only included in the git repository, and are
not bundled up with Regina's release tarballs.

Currently these bindings are intended for macOS, where clang and libclang are
provided by Xcode, but the corresponding python bindings are not.  For
GNU/Linux distributions, it should be straightforward to install matching
clang and python3-clang packages (and therefore the files here can be ignored).

The intention is to have a subdirectory for each major version of clang, so
that the python bindings can match the clang installation.
