Regina Census and Sample File Directory
---------------------------------------

This directory contains a number of censuses of triangulations of
different types.  Each census data file includes a text packet
containing a full description of the census including its origin.

Also within this directory is a number of sample files that illustrate how
Regina can be used.

The subdirectory sigs/ contains a series of plain text files representing
a census of splitting surface signatures.  See sigs/README.txt for further
details.

Files within this directory are of the following types.

*.rga:
    These are topology data files.  They can be opened directly from within
    the user interface.

*.session:
    These are complete transcripts of sample Jython sessions, including
    both user input and program output.  You can start a Jython session
    from the command line using "regina --console".

*.py:
    These are the same sample Jython sessions as above but only the user
    input has been included.  These scripts may be fed directly into Regina
    using a command of the form "regina --text < foo.py"

