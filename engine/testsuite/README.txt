Regina C++ Engine Test Suite
----------------------------

This directory tree contains the test suite for Regina's C++ calculation engine.
This test suite can be used to verify that Regina is operating correctly under
different compilers or on different platforms.

To build and run the test suite, type: make test ARGS=-V

This can be done from the top-level build directory, in which case it will run
all of Regina's test suites (the C++ engine, Python, and command-line utilities
tests).
