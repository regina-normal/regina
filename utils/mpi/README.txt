Regina MPI Utilities Directory
------------------------------

This directory contains the sources for command-line utilities that make
use of MPI (the Message Passing Interface, used for parallel computation).

As of Regina 5.1, these have been deprecated, and they will be removed in a
future release of Regina.

If you wish to parallelise the generation of a census, we recommend splitting
up the input pairing files into chunks, and using typical queue systems (such
as PBS) to parallelise.
