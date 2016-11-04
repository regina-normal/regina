Large census data files
-----------------------

This directory contains the raw data that mkcensusdb sources to create the
highly optimised census databases used by Census::lookup().

Each *.zsig file in this directory is a gzipped text file in the form:

<isosig> <manifold_name>
<isosig> <manifold_name>
...
<isosig> <manifold_name>

The extension .zsig is used (instead of something more sensible, like .sig.gz)
so that ${INPUT_FILE_BASE} does the right thing in xcode's custom build rules.

 -- Ben Burton <bab@maths.uq.edu.au>, Mon,  1 Sep 2014 18:12:12 +1000
