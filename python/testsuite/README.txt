Regina Python Test Suite
------------------------

This directory tree contains the test suite for Regina's python interface.
The test suite can be used to verify that Regina's python bindings are
operating correctly under different compilers or on different platforms.

To build and run the test suite, move to the top-level build directory
and type: make test ARGS=-V

The *.test scripts within this directory should, where possible, be written
to simultaneously work both as full scripts and also as line-by-line command
execution (simulating a human typing commands into a Python interpreter).
This puts extra requirements on the formatting and syntax; e.g.:
- blank lines must be added between successive indented blocks;
- blank lines within an indented block must include the indent also;
- return values should be assigned to dummy variables, not discarded
  (which would create spurious output in line-by-line execution).

See also the C++ test suite in ../../testsuite/.
