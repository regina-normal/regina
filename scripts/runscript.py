#!/bin/false
#
# Regina - A Normal Surface Theory Calculator
# Python Environment Initialisation
#
# Copyright (c) 2003, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# Usage: runscript.py [ <library> ... ] [ -- <script> [ <script-arg> ... ]]
#
# Initialises the python environment for working with the Regina
# calculation engine.  Tasks include:
#
#   - Importing the 'regina' module;
#   - Running a series of provided library scripts;
#   - Running a selected script with the provided command-line arguments,
#       or,
#     printing a startup banner for working in interactive mode.
#
# This script is for internal use only and should NOT be run directly.
# Instead the wrapper script regina-python should be used.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free
# Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307, USA.
#

import sys

# --- Import the Regina calculation engine. ---

try:
    import regina
except:
    print 'ERROR: The calculation engine module could not be loaded.'
    sys.exit(1)

# --- Parse the command-line arguments. ---

libNames = []
scriptName = None
scriptArgs = []

libsDone = 0
for i in sys.argv[1:]:
    if libsDone:
        if scriptName == None:
            scriptName = i
        else:
            scriptArgs.append(i)
    elif i == '--':
        libsDone = 1
    else:
        libNames.append(i)

# --- Run each library script in turn. ---

for i in libNames:
    try:
        print 'Running ' + i + '...'
        execfile(i)
    except SystemExit:
        pass
    except:
        sys.excepthook(*sys.exc_info())
        print "ERROR: The custom library '" + i + "' could not be executed."
        sys.exit(1)

# --- Run the script or print the banner as required. ---

if scriptName != None:
    sys.argv = [ scriptName ] + scriptArgs
    try:
        execfile(scriptName)
        sys.exit(0)
    except SystemExit:
        pass
    except:
        sys.excepthook(*sys.exc_info())
        print 'ERROR: An error occurred whilst executing the script.'
        sys.exit(1)
else:
    print regina.welcome()

