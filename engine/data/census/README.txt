Large census data files
-----------------------

This directory contains the raw data that mkcensusdb sources to create the
highly optimised census databases used by Census::lookup() / Census::lookupAs().

Each *.xzsig file in this directory is an xz-compressed text file in the form:

<sig> <manifold_or_link_name>
<sig> <manifold_or_link_name>
...
<sig> <manifold_or_link_name>

All signatures are second-generation isomorphism signatures or knot/link
signatures.

Each signature must appear only _once_ in each file.  If a signature has
several associated manifold/link names, then these name must all appear on the
same line, separated by tabs.  The manifold/link names themselves must contain
only printable characters and/or spaces.

Note: all triangulations and link diagrams in these databases must be connected.
(This is because the census lookup code uses the signature prefix to deduce
the triangulation/link size).

The extension .xzsig is used (instead of something more sensible, like .sig.xz)
so that ${INPUT_FILE_BASE} does the right thing in xcode's custom build rules.

 -- Ben Burton <bab@maths.uq.edu.au>, Thu, 28 May 2026 22:22:34 +1000
