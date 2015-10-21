# Regina - A Normal Surface Theory Calculator
# Python Module Initialisation
#
# Copyright (c) 2003-2014, Ben Burton
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
from .engine import *

def reginaSetup(quiet = False, readline = True, banner = False,
                snappyPath = True, libs = True, namespace = None):
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
        namespace  : The global namespace in which the start-up library scripts
                     (if any) will be executed.  This may be None, in which
                     case the caller's global namespace will be used.
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
            if sys.version_info[:2] == (2, 7):
                # Ask MacOS where SnapPy lives.
                import subprocess
                try:
                    app = subprocess.check_output(['mdfind',
                        'kMDItemDisplayName==SnapPy&&kMDItemKind==Application'])
                    if app:
                        app = app.strip().split('\n')[0]
                except:
                    app = None

                if not app:
                    app = '/Applications/SnapPy.app'
                snappyLib = app + '/Contents/Resources/lib/python2.7'
                if os.path.exists(snappyLib):
                    sys.path.append(snappyLib)
                    sys.path.append(snappyLib + '/site-packages.zip')
                    sys.path.append(snappyLib + '/lib-dynload')

    if libs:
        if __execLibs(namespace, quiet):
            # We tried to import something...
            if not quiet:
                # ... and we wrote some messages about it.
                print

    if banner:
        print engine.welcome()

def __execLibs(namespace = None, quiet = False):
    """
    For internal use by this module.
    Executes all of the user's start-up libraries in the global namespace.

    Arguments:
        namespace : The global namespace in which the library scripts will be
                    executed.  This may be None, in which case the caller's
                    global namespace will be used.
        quiet:      If true, do not print information about which libraries
                    are being loaded.

    Returns:
        True if there was at least one library that we tried to import,
        or False if there were none.
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
    triedImport = False
    for line in f:
        lib = line.strip()
        if len(lib) == 0:
            continue
        if lib[0] == '#':
            continue

        triedImport = True
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
            if namespace:
                execfile(lib, namespace)
            else:
                execfile(lib)
        except SystemExit:
            pass
        except:
            sys.excepthook(*sys.exc_info())
            print 'ERROR: Could not execute user library:', lib

    f.close()
    return triedImport

def __execScript(namespace = None):
    """
    For internal use by regina-python.
    Executes a given python script.

    The filename of the script should be in sys.argv[1], and
    any arguments to the script should be in sys.argv[2:].
    However, ipython sets things up a little differently (it includes two
    additional arguments), and we attempt to compensate for this here.

    SIDE-EFFECT: sys.argv will be truncated to include the script and
    its arguments only (i.e., sys.argv[0] will be removed).

    Arguments:
        namespace : The global namespace in which the script will be executed.
                    This may be None, in which case the caller's global
                    namespace will be used.
    """

    try:
        __IPYTHON__
        # Although python sets sys.argv = [ '-c', script, arg, ... ],
        # ipython sets sys.argv as the full original command line:
        #   [ ipython, '-c', command, script, arg, ... ], or
        #   [ ipython, '-i', '-c', command, script, arg, ... ].
        # Repair things here.
        if len(sys.argv) >= 3 and sys.argv[1] == '-c':
            sys.argv = [ '-c' ] + sys.argv[3:]
        elif len(sys.argv) >= 4 and sys.argv[1:3] == ['-i', '-c']:
            sys.argv = [ '-c' ] + sys.argv[4:]
    except:
        pass

    script = sys.argv[1]
    sys.argv = sys.argv[1:]
    if namespace:
        execfile(script, namespace)
    else:
        execfile(script)

