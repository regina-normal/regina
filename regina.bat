@echo off

rem ----------------------------------------------
rem |                                            |
rem |         Regina startup batch file          |
rem |    Copyright (c) Ben Burton, 1999-2002     |
rem |                                            |
rem |  Requires cygwin (http://www.cygwin.com/)  |
rem |                                            |
rem ----------------------------------------------

rem [ Begin error message]
rem
echo.
echo Regina startup batch file
echo -------------------------
echo You must edit this batch file before you can use it!
echo See %~f0 for details.
echo.
goto done
rem
rem [ End error message ]

rem EDITING THIS BATCH FILE
rem -----------------------
rem
rem 1) Change REG_CYGDRIVE and REG_CYGPATH below to reflect the location in
rem    which you installed cygwin.  This should be the directory containing
rem    cygwin1.dll and bash.exe.
rem
rem 2) Change REG_BIN below to reflect the cygwin directory in which
rem    the Regina binaries are installed.  This should be the directory
rem    containing this file (regina.bat), but it should be written in
rem    cygwin format.
rem
rem    Thus, if the Regina binaries are in C:\cygwin\usr\local\bin then
rem    you should use /usr/local/bin.  Note that /cygdrive/c refers
rem    to C:\ (and /cygdrive/d to D:\ and so on), so if Regina is installed
rem    in C:\apps\regina then you can use /cygdrive/c/apps/regina.
rem
rem 3) Delete the block above between "Begin error message" and "End error
rem    message" so the batch file will run normally.
rem
rem If any of these paths contain spaces, this is fine.  Do not use quotes
rem yourself - the startup scripts will take care of such details.
rem
rem Please mail me (benb@acm.org) if you have any questions.

set REG_CYGDRIVE=C:
set REG_CYGPATH=\cygwin\bin
set REG_BIN=/usr/local/bin

rem --- You should not edit anything below this point.

rem --- Move to the cygwin /usr/bin directory.

%REG_CYGDRIVE%
pushd .
cd %REG_CYGPATH%

rem --- Start regina.  This should call the startup script "regina" in
rem     the base regina directory.

bash --login "%REG_BIN%/regina" %*

rem --- Pause so the user can see any text output before the console is
rem     closed; remove this if appropriate.

popd
:done
pause

