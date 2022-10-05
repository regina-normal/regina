#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  Syntax: mkdoc.py [-I<path> ..] [.. a list of header files ..]
#
#  Extract documentation from C++ header files to use it in Python bindings
#

import os
import sys
import platform
import re
import textwrap

import ctypes.util

from clang import cindex
from clang.cindex import CursorKind, AccessSpecifier, AvailabilityKind
from collections import OrderedDict
from glob import glob
from multiprocessing import cpu_count

RECURSE_LIST = [
    CursorKind.TRANSLATION_UNIT,
    CursorKind.NAMESPACE,
    CursorKind.CLASS_DECL,
    CursorKind.STRUCT_DECL,
    CursorKind.ENUM_DECL,
    CursorKind.CLASS_TEMPLATE
]

PRINT_LIST = [
    CursorKind.CLASS_DECL,
    CursorKind.STRUCT_DECL,
    CursorKind.ENUM_DECL,
    CursorKind.ENUM_CONSTANT_DECL,
    CursorKind.CLASS_TEMPLATE,
    CursorKind.FUNCTION_DECL,
    CursorKind.FUNCTION_TEMPLATE,
    CursorKind.CONVERSION_FUNCTION,
    CursorKind.CXX_METHOD,
    CursorKind.CONSTRUCTOR,
    CursorKind.FIELD_DECL
]

PREFIX_BLACKLIST = [
    CursorKind.TRANSLATION_UNIT
]

ACCESS_BLACKLIST = [
    AccessSpecifier.PRIVATE,
    AccessSpecifier.PROTECTED
]

AVAILABILITY_BLACKLIST = [
    AvailabilityKind.NOT_AVAILABLE
]

CLASS_BLACKLIST = [
]

MEMBER_BLACKLIST = [
    'operator=', 'writeTextShort', 'writeTextLong'
]

CPP_OPERATORS = {
    '<=': 'le', '>=': 'ge', '==': 'eq', '!=': 'ne', '[]': 'array',
    '+=': 'iadd', '-=': 'isub', '*=': 'imul', '/=': 'idiv', '%=':
    'imod', '&=': 'iand', '|=': 'ior', '^=': 'ixor', '<<=': 'ilshift',
    '>>=': 'irshift', '++': 'inc', '--': 'dec', '<<': 'lshift', '>>':
    'rshift', '&&': 'land', '||': 'lor', '!': 'lnot', '~': 'bnot',
    '&': 'band', '|': 'bor', '+': 'add', '-': 'sub', '*': 'mul', '/':
    'div', '%': 'mod', '<': 'lt', '>': 'gt', '=': 'assign', '()': 'call'
}

CPP_OPERATORS = OrderedDict(
    sorted(CPP_OPERATORS.items(), key=lambda t: -len(t[0])))

errors_detected = False
docstring_width = int(70)


class NoFilenamesError(ValueError):
    pass


def d(s):
    return s if isinstance(s, str) else s.decode('utf8')


def sanitize_name(name):
    name = re.sub(r'type-parameter-0-([0-9]+)', r'T\1', name)
    for k, v in CPP_OPERATORS.items():
        name = name.replace('operator%s' % k, '__%s' % v)
    name = re.sub('<.*>', '', name)
    name = ''.join([ch if ch.isalnum() else '_' for ch in name])
    # name = re.sub('_$', '', re.sub('_+', '_', name))
    return name


def process_comment(comment):
    result = ''

    # Remove C++ comment syntax
    leading_spaces = float('inf')
    for s in comment.expandtabs(tabsize=4).splitlines():
        s = s.strip()
        if s.startswith('/*'):
            s = s[2:].lstrip('*')
        elif s.endswith('*/'):
            s = s[:-2].rstrip('*')
        elif s.startswith('///'):
            s = s[3:]
        if s.startswith('*'):
            s = s[1:]
        if len(s) > 0:
            leading_spaces = min(leading_spaces, len(s) - len(s.lstrip()))
        result += s + '\n'

    if leading_spaces != float('inf'):
        result2 = ""
        for s in result.splitlines():
            result2 += s[leading_spaces:] + '\n'
        result = result2

    # Doxygen tags
    cpp_group = r'([\w:]+)'
    param_group = r'([\[\w:,\]]+)'

    s = result
    s = re.sub(r'[\\@]c\s+%s' % cpp_group, r'``\1``', s)
    s = re.sub(r'[\\@]a\s+%s' % cpp_group, r'*\1*', s)
    s = re.sub(r'[\\@]e\s+%s' % cpp_group, r'*\1*', s)
    s = re.sub(r'[\\@]em\s+%s' % cpp_group, r'*\1*', s)
    s = re.sub(r'[\\@]b\s+%s' % cpp_group, r'**\1**', s)
    s = re.sub(r'[\\@]ingroup\s+%s' % cpp_group, r'', s)
    s = re.sub(r'[\\@]param%s?\s+%s' % (param_group, cpp_group),
               r'\n\n$Parameter ``\2``:\n\n', s)
    s = re.sub(r'[\\@]tparam%s?\s+%s' % (param_group, cpp_group),
               r'\n\n$Template parameter ``\2``:\n\n', s)

    for in_, out_ in {
        'returns': 'Returns',
        'return': 'Returns',
        'authors': 'Authors',
        'author': 'Author',
        'copyright': 'Copyright',
        'date': 'Date',
        'remark': 'Remark',
        'sa': 'See also',
        'see': 'See also',
        'extends': 'Extends',
        'pre': 'Precondition',
        'ifacespython': 'Python',
        'ifacescpp': 'C++',
        'i18n': 'Internationalisation',
        'exception': 'Exceptions',
        'throws': 'Throws',
        'throw': 'Throws'
    }.items():
        s = re.sub(r'[\\@]%s\s*' % in_, r'\n\n$%s:\n\n' % out_, s)

    s = re.sub(r'[\\@]details\s*', r'\n\n', s)
    s = re.sub(r'[\\@]brief\s*', r'', s)
    s = re.sub(r'[\\@]short\s*', r'', s)
    s = re.sub(r'[\\@]ref\s*', r'', s)

    s = re.sub(r'[\\@]code\s?(.*?)\s?[\\@]endcode',
               r"```\n\1\n```\n", s, flags=re.DOTALL)
    s = re.sub(r'[\\@]warning\s?(.*?)\s?\n\n',
               r'$.. warning::\n\n\1\n\n', s, flags=re.DOTALL)
    s = re.sub(r'[\\@]note\s?(.*?)\s?\n\n',
               r'$.. note::\n\n\1\n\n', s, flags=re.DOTALL)
    # Deprecated expects a version number for reST and not for Doxygen. Here the first word of the
    # doxygen directives is assumed to correspond to the version number
    s = re.sub(r'[\\@]deprecated\s(.*?)\s?(.*?)\s?\n\n',
               r'$.. deprecated:: \1\n\n\2\n\n', s, flags=re.DOTALL)
    s = re.sub(r'[\\@]since\s?(.*?)\s?\n\n',
               r'.. versionadded:: \1\n\n', s, flags=re.DOTALL)
    s = re.sub(r'[\\@]todo\s?(.*?)\s?\n\n',
               r'$.. todo::\n\n\1\n\n', s, flags=re.DOTALL)

    # HTML/TeX tags
    s = re.sub(r'<tt>(.*?)</tt>', r'``\1``', s, flags=re.DOTALL)
    s = re.sub(r'<pre>(.*?)</pre>', r"```\n\1\n```\n", s, flags=re.DOTALL)
    s = re.sub(r'<em>(.*?)</em>', r'*\1*', s, flags=re.DOTALL)
    s = re.sub(r'<i>(.*?)</i>', r'*\1*', s, flags=re.DOTALL)
    s = re.sub(r'<b>(.*?)</b>', r'**\1**', s, flags=re.DOTALL)
    s = re.sub(r'[\\@]f\$(.*?)[\\@]f\$', r':math:`\1`', s, flags=re.DOTALL)
    s = re.sub(r'<li>', r'\n\n* ', s)
    s = re.sub(r'</?ul>', r'', s)
    s = re.sub(r'</li>', r'\n\n', s)

    # Special characters
    s = re.sub(r'\\<', r'<', s)
    s = re.sub(r'\\>', r'>', s)
    s = re.sub(r'\\&', r'&', s)

    s = s.replace('``true``', '``True``')
    s = s.replace('``false``', '``False``')

    # Re-flow text
    wrapper = textwrap.TextWrapper()
    wrapper.expand_tabs = True
    wrapper.replace_whitespace = True
    wrapper.drop_whitespace = True
    wrapper.width = docstring_width
    wrapper.initial_indent = wrapper.subsequent_indent = ''

    result = ''
    in_code_segment = False
    for x in re.split(r'(```)', s):
        if x == '```':
            if not in_code_segment:
                result += '```\n'
            else:
                result += '\n```\n\n'
            in_code_segment = not in_code_segment
        elif in_code_segment:
            result += x.strip()
        else:
            # Split into paragraphs.
            for y in re.split(r'(?: *\n *){2,}', x):
                # Split out any list items in this paragraph.
                # A paragraph has optional plain text, followed by one or more
                # optional list items.  (In particular, the list items can
                # never be followed by additional plain text.)
                #
                # Note for later: when we get to detecting ordered lists,
                # these should be rendered in the docstring as: #. <text>
                list_indents = []
                for z in re.split(r'(?:^|\n)(\s*[-+*]\s+)', y):
                    # The list bullets, including their initial indents, will
                    # appear as separate pieces of the split we just performed.
                    if len(z) == 0:
                        continue
                    zstrip = z.lstrip()
                    if len(zstrip) >= 2 and zstrip[:2] in [ '- ', '+ ', '* ' ]:
                        # This piece of the split is a bullet.
                        # Work out its indent and loop again to fetch
                        # the actual list item text.
                        indent = len(z) - len(zstrip)
                        if not list_indents:
                            list_indents = [ indent ]
                        elif list_indents[-1] < indent:
                            list_indents.append(indent)
                        else:
                            while list_indents and list_indents[-1] > indent:
                                list_indents.pop()
                            # TODO: Maybe it would be polite to check here
                            # whether our reduced indent matches a
                            # previously-seen indent level.
                        continue

                    if list_indents:
                        wrapper.initial_indent = '  ' * (len(list_indents) - 1)
                        wrapper.subsequent_indent = '  ' * len(list_indents)
                        z = '* ' + z

                    wrapped = wrapper.fill(re.sub(r'\s+', ' ', z).strip())

                    if len(wrapped) > 0 and wrapped[0] == '$':
                        # TODO: Maybe it would be nice to verify that
                        # we do not also have a list indent at this point,
                        # since special paragraphs and list items do not
                        # play well together.
                        result += wrapped[1:] + '\n'
                        wrapper.initial_indent = \
                            wrapper.subsequent_indent = ' ' * 4
                    else:
                        if len(wrapped) > 0:
                            result += wrapped + '\n\n'
                        wrapper.initial_indent = wrapper.subsequent_indent = ''
    return result.rstrip().lstrip('\n')


def extract(filename, node, namespace, output):
    if not (node.location.file is None or
            os.path.samefile(d(node.location.file.name), filename)):
        return 0

    if node.raw_comment and '\\ifacespython Not present.\n' in node.raw_comment:
        # It is clear in the C++ docs that this entity has no Python binding.
        return

    if node.kind in RECURSE_LIST and node.spelling not in CLASS_BLACKLIST:
        sub_namespace = namespace
        if node.kind not in PREFIX_BLACKLIST:
            # Ignore the leading regina:: namespace, which everything has.
            if not (node.kind == CursorKind.NAMESPACE and \
                    node.spelling == 'regina' and namespace == ''):
                if len(namespace) > 0:
                    sub_namespace += '::'
                sub_namespace += sanitize_name(d(node.spelling))
                # When delving into the class/struct/enum X, use the
                # namespace X_ for the members of X.
                if node.kind != CursorKind.NAMESPACE:
                    sub_namespace += '_'
        for i in node.get_children():
            extract(filename, i, sub_namespace, output)
    if node.kind in PRINT_LIST and \
            node.access_specifier not in ACCESS_BLACKLIST and \
            node.availability not in AVAILABILITY_BLACKLIST and \
            node.spelling not in MEMBER_BLACKLIST and \
            node.spelling not in CLASS_BLACKLIST and \
            (not node.is_move_constructor()):
        sub_namespace = namespace
        if len(node.spelling) > 0:
            name = sanitize_name(d(node.spelling))

            fullname = 'regina::'
            if namespace:
                fullname = fullname + namespace + '::'
            fullname += name

            # if node.lexical_parent != node.semantic_parent:
            if node != node.canonical:
                # We are seeing functions with inline definitions and/or
                # forward declarations appear multiple times in the output.
                # Try to ensure that their docstrings are listed only once.
                return

            if node.raw_comment is None:
                print('    Undocumented:', fullname, '-- skipping')
                return

            if node.spelling == 'operator<<':
                # We do not want docs for std::ostream output operators.
                # For now we skip *all* left shift operators; this may need to
                # become more nuanced at a later date.
                print('    Left shift:', fullname, '-- skipping')
                return

            comment = d(node.raw_comment)
            comment = process_comment(comment)
            output.append((sub_namespace, name, filename, comment))


def read_args(args):
    parameters = []
    filenames = []
    if "-x" not in args:
        parameters.extend(['-x', 'c++'])
    if not any(it.startswith("-std=") for it in args):
        parameters.append('-std=c++11')
    parameters.append('-Wno-pragma-once-outside-header')
    parameters.append('-D__DOCSTRINGS')

    if platform.system() == 'Darwin':
        dev_path = '/Applications/Xcode.app/Contents/Developer/'
        lib_dir = dev_path + 'Toolchains/XcodeDefault.xctoolchain/usr/lib/'
        sdk_dir = dev_path + 'Platforms/MacOSX.platform/Developer/SDKs'
        libclang = lib_dir + 'libclang.dylib'

        if os.path.exists(libclang):
            cindex.Config.set_library_path(os.path.dirname(libclang))

        if os.path.exists(sdk_dir):
            sysroot_dir = os.path.join(sdk_dir, next(os.walk(sdk_dir))[1][0])
            parameters.append('-isysroot')
            parameters.append(sysroot_dir)
    elif platform.system() == 'Windows':
        if 'LIBCLANG_PATH' in os.environ:
            library_file = os.environ['LIBCLANG_PATH']
            if os.path.isfile(library_file):
                cindex.Config.set_library_file(library_file)
            else:
                raise FileNotFoundError("Failed to find libclang.dll! "
                                        "Set the LIBCLANG_PATH environment variable to provide a path to it.")
        else:
            library_file = ctypes.util.find_library('libclang.dll')
            if library_file is not None:
                cindex.Config.set_library_file(library_file)
    elif platform.system() == 'Linux':
        # LLVM switched to a monolithical setup that includes everything under
        # /usr/lib/llvm{version_number}/. We glob for the library and select
        # the highest version
        def folder_version(d):
            return [int(ver) for ver in re.findall(r'(?<!lib)(?<!\d)\d+', d)]

        llvm_dir = max((
            path
            for libdir in ['lib64', 'lib', 'lib32']
            for path in glob('/usr/%s/llvm-*' % libdir)
            if os.path.exists(os.path.join(path, 'lib', 'libclang.so.1'))
        ), default=None, key=folder_version)

        # Ability to override LLVM/libclang paths
        if 'LLVM_DIR_PATH' in os.environ:
            llvm_dir = os.environ['LLVM_DIR_PATH']
        elif llvm_dir is None:
            raise FileNotFoundError(
                "Failed to find a LLVM installation providing the file "
                "/usr/lib{32,64}/llvm-{VER}/lib/libclang.so.1. Make sure that "
                "you have installed the packages libclang1-{VER} and "
                "libc++-{VER}-dev, where {VER} refers to the desired "
                "Clang/LLVM version (e.g. 11). You may alternatively override "
                "the automatic search by specifying the LIBLLVM_DIR_PATH "
                "(for the LLVM base directory) and/or LIBCLANG_PATH (if "
                "libclang is located at a nonstandard location) environment "
                "variables.")

        if 'LIBCLANG_PATH' in os.environ:
            libclang_dir = os.environ['LIBCLANG_PATH']
        else:
            libclang_dir = os.path.join(llvm_dir, 'lib', 'libclang.so.1')

        cindex.Config.set_library_file(libclang_dir)
        cpp_dirs = [ ]

        if '-stdlib=libc++' not in args:
            cpp_dirs.append(max(
                glob('/usr/include/c++/*'
            ), default=None, key=folder_version))

            cpp_dirs.append(max(
                glob('/usr/include/%s-linux-gnu/c++/*' % platform.machine()
            ), default=None, key=folder_version))
        else:
            cpp_dirs.append(os.path.join(llvm_dir, 'include', 'c++', 'v1'))

        if 'CLANG_INCLUDE_DIR' in os.environ:
            cpp_dirs.append(os.environ['CLANG_INCLUDE_DIR'])
        else:
            cpp_dirs.append(max(
                glob(os.path.join(llvm_dir, 'lib', 'clang', '*', 'include')
            ), default=None, key=folder_version))

        cpp_dirs.append('/usr/include/%s-linux-gnu' % platform.machine())
        cpp_dirs.append('/usr/include')

        # Capability to specify additional include directories manually
        if 'CPP_INCLUDE_DIRS' in os.environ:
            cpp_dirs.extend([cpp_dir for cpp_dir in os.environ['CPP_INCLUDE_DIRS'].split()
                             if os.path.exists(cpp_dir)])

        for cpp_dir in cpp_dirs:
            if cpp_dir is None:
                continue
            parameters.extend(['-isystem', cpp_dir])

    for item in args:
        if item.startswith('-'):
            parameters.append(item)
        else:
            filenames.append(item)

    if len(filenames) == 0:
        raise NoFilenamesError("args parameter did not contain any filenames")

    return parameters, filenames


def extract_all(args):
    parameters, filenames = read_args(args)
    output = []

    global errors_detected
    for filename in filenames:
        print('Processing "%s" ..' % filename, file=sys.stderr)
        try:
            index = cindex.Index(
                cindex.conf.lib.clang_createIndex(False, True))
            tu = index.parse(filename, parameters)
            extract(filename, tu.cursor, '', output)
        except BaseException:
            errors_detected = True
            raise

    return output


def write_header(comments, out_file=sys.stdout):
    print('''/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {
''', file=out_file)


    name_ctr = 1
    name_prev = None
    namespace_prev = None
    for namespace, name, _, comment in list(sorted(comments, key=lambda x: (x[0], x[1], x[2]))):
        if (namespace, name) == name_prev:
            name_ctr += 1
            name = name + "_%i" % name_ctr
        else:
            name_prev = (namespace, name)
            name_ctr = 1

        if namespace != namespace_prev:
            if namespace_prev:
                print('\n}', file=out_file)
            if namespace:
                print('\nnamespace %s {' % namespace, file=out_file)
            namespace_prev = namespace

        full_namespace = 'regina::python::doc'
        if namespace:
            full_namespace = full_namespace + '::' + namespace
        print('\n// Docstring %s::%s' % (full_namespace, name), file=out_file)
        print('static const char *%s =%sR"doc(%s)doc";' %
              (name, '\n' if '\n' in comment else ' ', comment), file=out_file)

    if namespace_prev:
        print('\n}', file=out_file)

    print('''
} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif
''', file=out_file)


def mkdoc(args, width, output=None):
    if width != None:
        global docstring_width
        docstring_width = int(width)
    comments = extract_all(args)
    if errors_detected:
        return

    if output:
        try:
            os.makedirs(os.path.dirname(os.path.abspath(output)), exist_ok=True)
            with open(output, 'w') as out_file:
                write_header(comments, out_file)
        except:
            # In the event of an error, don't leave a partially-written
            # output file.
            try:
                os.unlink(output)
            except:
                pass
            raise
    else:
        write_header(comments)
