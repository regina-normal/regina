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

The extension .xzsig is used (instead of something more sensible, like .sig.xz)
so that ${INPUT_FILE_BASE} does the right thing in xcode's custom build rules.

 -- Ben Burton <bab@maths.uq.edu.au>, Tue, 26 May 2026 16:25:08 +1000
