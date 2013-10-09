Pre-built configuration headers
-------------------------------

The header regina-config.h is created in the build directory by cmake,
and its contents are based on cmake's analysis of the build system.

For builds that (for whatever reason) are not using cmake, this
directory contains some ready-made versions of regina-config.h.
Simply add the corresponding subdirectory to your header include path.

Caveats:

- These configuration headers are not for general use, and have
  only been verified on the developers' specific build machine setup.

- The Regina version is hard-coded into these headers, and will need to
  be updated with each new release.

