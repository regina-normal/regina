@echo off

rem ----------------------------------------------
rem |                                            |
rem |         Regina startup batch file          |
rem |       Copyright (c) Ben Burton, 2001       |
rem |                                            |
rem |  Requires cygwin (http://www.cygwin.com/)  |
rem |                                            |
rem ----------------------------------------------

rem --- Move to the cygwin /usr/bin directory; change this if appropriate.

C:
pushd .
cd \cygwin\bin

rem --- Start regina.  This should call the startup script "regina" in
rem     the base regina directory.

bash --login -c "/home/Ben/source/regina/regina %*"

rem --- Pause so the user can see any text output before the console is
rem     closed; remove this if appropriate.

popd
pause

