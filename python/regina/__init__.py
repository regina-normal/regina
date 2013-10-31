# Regina - A Normal Surface Theory Calculator
# Python Module Initialisation
#
# Copyright (c) 2003-2013, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
# MA 02110-1301, USA.
#

import sys, os
from .regina import *

def reginaSetup(quiet = False, readline = True, banner = False,
                snappyPath = True, libs = True):
    """
    Initialise a Regina python session.

    Arguments:
        quiet      : If true, suppress informative messages that would
                     otherwise be written to standard output.  If an error
                     occurs, details of the error will be written regardless.
        readline   : If true, attempt to enable tab completion.
        libs       : If true, execute all of the user's start-up libraries
                     in the global namespace.
        snappyPath : Applies to platforms where SnapPy might not be installed
                     on the python path (e.g., MacOS users with the SnapPy app
                     bundle).  If true, this setup routine will (i) attempt
                     to locate SnapPy, and (ii) if successful, extend sys.path
                     to include the location of SnapPy's python module and its
                     dependencies.
        banner     : If true, print a welcome banner to standard output.
    """

    if readline:
        # Enable tab completion through readline, if we can.
        try:
            import rlcompleter, readline
            # readline by default completes an empty string, whereas if 
            # we press tab we want to insert an actual tab character, 
            # so we have our own completion function.

            __internal_python_completer = readline.get_completer()
            def regina_completer(text, state):
              if not text:
                return ('\t', None)[state]
              else:
                return __internal_python_completer(text, state)
            readline.set_completer(regina_completer)

            if 'libedit' in readline.__doc__:
                # Some systems work with libedit, not libreadline, which
                # supports a different set of commands.
                readline.parse_and_bind('bind ^I rl_complete')
            else:
                readline.parse_and_bind('tab: complete')
        except:
            pass

    if snappyPath:
        # For the time being, only find SnapPy on MacOS.
        if sys.platform == 'darwin':
            if sys.version_info.major == 2 and sys.version_info.minor == 7:
                # Ask MacOS where SnapPy lives.
                import subprocess
                try:
                    app = subprocess.check_output(['mdfind',
                        'kMDItemDisplayName==SnapPy&&kMDItemKind==Application'])
                    if app:
                        app = app.strip()
                except:
                    app = None

                if not app:
                    app = '/Applications/SnapPy.app'
                snappyLib = app + '/Contents/Resources/lib/python2.7'
                if os.path.exists(snappyLib):
                    sys.path.append(snappyLib)
                    sys.path.append(snappyLib + '/site-packages.zip')

    if libs:
        __execLibs(quiet)

    if banner:
        print regina.welcome()

def __execLibs(quiet = False):
    """
    For internal use by this module.
    Executes all of the user's start-up libraries in the global namespace.

    Arguments:
        quiet: If true, do not print information about which libraries
               are being loaded.
    """

    libConfig = os.path.expanduser('~/.regina-libs')

    try:
        f = open(libConfig, 'r')
    except:
        # No configuration file.  Silently do nothing.
        return

    # The filenames in the configuration file are encoded using UTF-8.
    # Find the correct encoding for talking with the filesystem.
    import locale
    codeset = locale.getdefaultlocale()[1]
    if codeset == 'UTF-8':
        codeset = None

    # If something goes wrong whilst reading the file, let the exception
    # fall through to the user.
    for line in f:
        lib = line.strip()
        if len(lib) == 0:
            continue
        if lib[0] == '#':
            continue

        if codeset:
            lib = lib.decode('UTF-8').encode(codeset)

        if not os.path.exists(lib):
            print 'ERROR: Python library does not exist: ' + lib
            print '       Please remove this from the configuration file: ' + \
                libConfig
            continue

        if not quiet:
            print 'Running ' + lib + '...'
        try:
            execfile(lib, globals())
        except SystemExit:
            pass
        except:
            sys.excepthook(*sys.exc_info())
            print 'ERROR: Could not execute user library:', lib
            sys.exit(1)

    f.close()

def __execScript():
    """
    For internal use by regina-python.
    Executes a given python script in the global namespace.

    The filename of the script must be in sys.argv[1], and
    any arguments to the script must be in sys.argv[2:].

    SIDE-EFFECT: sys.argv will be truncated to include the script
    arguments only (i.e., sys.argv[0] and sys.argv[1] will be removed).
    """

    script = sys.argv[1]
    sys.argv = sys.argv[2:]
    execfile(script, globals())

