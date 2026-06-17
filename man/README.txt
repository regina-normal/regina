Regina Manual Pages
-------------------

This directory is used to generate man pages that accompany Regina's
main executables and other command-line utilities.

The man pages are extracted from the Regina handbook, which is located
in ../handbook/regina/.

Typically only the Regina developers would generate these man pages:
this is not part of the usual build process (instead the pre-generated
man pages are shipped as part of Regina's source tarball).

To generate the man pages, one needs to run "make -f Makefile.man man"
(which requires a working docbook2man installation), and then manually
tweak the results (which currently involves replacing "    ..." with
"    \&..." at the beginnings of lines showing sample input or output).
The date at the top of each man page should reflect the date on which
the man page was generated.

 -- Ben Burton <bab@debian.org>, Thu, 11 Jun 2026 01:02:36 +1000
