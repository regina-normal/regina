# Regina - A Normal Surface Theory Calculator
# Python Test Suite Component
#
# Copyright (c) 2015-2025, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# Prints basic properties of all wrapped objects and types.
#
# This file is a single component of Regina's python test suite.  To run
# the python test suite, move to the main python directory in the source
# tree and run "make check".
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
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import re

regexClass = re.compile(r"^<class 'regina\.engine\.(.+)'>$")
regexFuncSynopsis = re.compile(r"^(\d+\. )?(\S+)\((.*)\) -> (\S.*\S)$", re.MULTILINE)

CLASS_MEMBER_BLACKLIST = [ '__doc__', '__hash__', '__module__',
    '__weakref__', 'equalityType' ]

FUNCTION_BLACKLIST = [ 'reginaSetup', '__execScript' ]

def checkFunctionArg(name, argType):
    # Strip out default values.
    pos = argType.find(' = ')
    if pos >= 0:
        argType = argType[:pos]

    if '<' in argType:
        stripped = argType.replace('<internal>', '')
        if '<' in stripped:
            print('TEMPLATE TYPE:', name, ':', argType)
    if 'regina.engine' in argType:
        print('ENGINE TYPE:', name, ':', argType)


def checkFunction(name, member):
    # print(name, ': function')
    doc = member.__doc__
    if not doc:
        print('ERROR:', name, 'No docstring')
        return

    foundSynopsis = False
    for m in regexFuncSynopsis.finditer(doc):
        foundSynopsis = True
        # m[3] is the list of arguments; m[4] is the return type.
        if len(m[3]) > 0:
            args = re.split(r'(?:^|, )[A-Za-z0-9_]+: ', m[3])
            bad = False
            if not args:
                bad = True
            elif args[0] != '':
                bad = True
            else:
                for a in args[1:]:
                    checkFunctionArg(name, a)
            if bad:
                print('ERROR:', name, 'Bad argument list:', m[3])
        checkFunctionArg(name, m[4])
    if not foundSynopsis:
        print('ERROR:', name, 'No synopsis')

def checkClass(name, member):
    m = regexClass.match(str(member))
    if not m:
        print(name, ': ERROR - could not determine real class name.')
    else:
        realName = m[1]
        # If realName != name then we have a type alias, so ignore it.
        if realName == name:
            if name == 'Bitmask256' or name == 'Qitmask256':
                # These classes are only present on 64-bit machines.
                # Omit them here so that this test generates the same output
                # across all platforms.
                return
            for key, sub in member.__dict__.items():
                if key in CLASS_MEMBER_BLACKLIST:
                    continue
                if isinstance(sub, type):
                    # Some class enums show up here.
                    try:
                        tmp = sub.__members__
                        # We have a class enum.
                    except AttributeError:
                        print('ERROR: Unknown inner type:', name + '.' + key)
                elif 'staticmethod' in str(sub.__class__):
                    checkFunction(name + '.' + key, sub.__get__(member))
                elif 'instancemethod' in str(sub.__class__):
                    checkFunction(name + '.' + key, sub)
                elif 'method' in str(sub.__class__):
                    print('ERROR: Unknown method type:', sub.__class__)
                elif 'static_property' in str(sub.__class__):
                    pass
                elif 'wrapper_descriptor' in str(sub.__class__):
                    # I think these are disabled constructors for
                    # abstract base classes.
                    pass

for key, member in regina.__dict__.items():
    if isinstance(member, type):
        try:
            tmp = member.__members__ # Exists for enums, not classes
            # We ignore enums for now.
            pass
        except AttributeError:
            checkClass(key, member)
    elif 'builtin_function_or_method' in str(member.__class__):
        checkFunction(key, member)
    elif 'function' in str(member.__class__) and key not in FUNCTION_BLACKLIST:
        print('ERROR: Unknown function type:', key, member.__class__)
    elif 'method' in str(member.__class__):
        print('ERROR: Unknown method type:', key, member.__class__)
    # Otherwise the member should be a module-level constant.
    # Ignore this also.

