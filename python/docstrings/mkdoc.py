#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  Syntax: mkdoc.py [-I<path> ..] [.. a list of header files ..]
#
#  Extract documentation from C++ header files to use it in Python bindings
#

import argparse
import os
import sys
import platform
import shlex
import re
import textwrap

import ctypes.util

from clang import cindex
from clang.cindex import CursorKind, TypeKind, AccessSpecifier, AvailabilityKind
from collections import OrderedDict
from glob import glob
from multiprocessing import cpu_count

__version__ = "2.6.2.dev1.regina"

INLINE_FILES = [
    '../../engine/census/gluingperms.h',
    '../../engine/core/output.h',
    '../../engine/triangulation/example.h',
    '../../engine/triangulation/isosigencoding.h',
    '../../engine/triangulation/isosigtype.h',
    '../../engine/utilities/flags.h',
    '../../engine/utilities/listview.h',
    '../../engine/utilities/snapshot.h',
    '../../engine/utilities/tableview.h'
]

INLINE_DIRS = [
    '../../engine/triangulation/alias',
    '../../engine/triangulation/detail',
    '../../engine/triangulation/generic'
]

RECURSE_LIST = [
    CursorKind.TRANSLATION_UNIT,
    CursorKind.NAMESPACE,
    CursorKind.CLASS_DECL,
    CursorKind.STRUCT_DECL,
    CursorKind.ENUM_DECL,
    CursorKind.CLASS_TEMPLATE,
    CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION
]

PRINT_LIST = [
    CursorKind.CLASS_DECL,
    CursorKind.STRUCT_DECL,
    CursorKind.ENUM_DECL,
    CursorKind.ENUM_CONSTANT_DECL,
    CursorKind.CLASS_TEMPLATE,
    CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION,
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

INLINE_DUPLICATES = [
    CursorKind.CXX_METHOD,
    CursorKind.CONSTRUCTOR,
    CursorKind.FUNCTION_TEMPLATE
]

ACCESS_BLACKLIST = [
    AccessSpecifier.PRIVATE,
    AccessSpecifier.PROTECTED
]

AVAILABILITY_BLACKLIST = [
    AvailabilityKind.NOT_AVAILABLE
]

NAMESPACE_BLACKLIST = [
]

CLASS_BLACKLIST = [
]

CLASS_WHITELIST = [
    'SnLookup', 'S1Lookup', 'S2Lookup', 'S3Lookup', 'S4Lookup',
    'S5Lookup', 'S6Lookup', 'S7Lookup',
    'OrderedSnLookup', 'OrderedS3Lookup', 'OrderedS4Lookup', 'OrderedS5Lookup',
    'OrderedS6Lookup', 'OrderedS7Lookup', 'OrderedSnLookup'
]

MEMBER_BLACKLIST = [
    'operator='
]

CPP_OPERATORS = {
    '<=': 'le', '>=': 'ge', '==': 'eq', '!=': 'ne', '[]': 'array',
    '+=': 'iadd', '-=': 'isub', '*=': 'imul', '/=': 'idiv', '%=':
    'imod', '&=': 'iand', '|=': 'ior', '^=': 'ixor', '<<=': 'ilshift',
    '>>=': 'irshift', '++': 'inc', '--': 'dec', '<<': 'lshift', '>>':
    'rshift', '&&': 'land', '||': 'lor', '!': 'lnot', '~': 'bnot', '&': 'band',
    '|': 'bor', '^': 'bxor', '+': 'add', '-': 'sub', '*': 'mul', '/':
    'div', '%': 'mod', '<': 'lt', '>': 'gt', '=': 'assign', '()': 'call',
    ' bool' : 'bool'
}

CPP_OPERATORS = OrderedDict(
    sorted(CPP_OPERATORS.items(), key=lambda t: -len(t[0])))

errors_detected = False
docstring_width = int(70)

inline = False
printed = []

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


def process_comment(comment, preserveAmpersands):
    result = ''

    # Remove C++ comment syntax
    leading_spaces = float('inf')
    verb_indent = None
    for s in comment.expandtabs(tabsize=4).splitlines():
        init_len = len(s)
        s = s.strip()
        ignore_indent = False
        if s.startswith('/*'):
            s = s[2:].lstrip('*')
        elif s.endswith('*/'):
            s = s[:-2].rstrip('*')
        elif s.startswith('///'):
            s = s[3:]
        if s.startswith('*'):
            s = s[1:]
        # This code assumes that verbatim blocks, *including* the
        # \verbatim and \endverbatim lines, will not contain the leading '*'
        # in their comment lines.  This messes with our indent calculations,
        # and so we work our way around that here.
        if s.startswith('\\verbatim'):
            verb_indent = init_len - len(s)
            ignore_indent = True
        elif s.startswith('\\endverbatim'):
            verb_indent = None
            ignore_indent = True
        elif verb_indent != None:
            # The leading spaces might be deliberate.
            # Put them back, but only as far as needed to align with the
            # \verbatim command.
            ignore_indent = True
            if len(s) + verb_indent < init_len:
                s = (' ' * (init_len - len(s) - verb_indent)) + s
        if ignore_indent:
            # We are going to strip off leading spaces shortly, so put
            # them back for now.
            if leading_spaces != float('inf'):
                s = (' ' * leading_spaces) + s
        else:
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
    s = re.sub(r'[\\@]exception\s+%s' % cpp_group,
               r'\n\n$Exception ``\1``:\n\n', s)

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
        'snappy': 'SnapPy',
        'i18n': 'Internationalisation'
    }.items():
        s = re.sub(r'[\\@]%s\s*' % in_, r'\n\n$%s:\n\n' % out_, s)

    s = re.sub(r'[\\@]details\s*', r'\n\n', s)
    s = re.sub(r'[\\@]brief\s*', r'', s)
    s = re.sub(r'[\\@]short\s*', r'', s)

    # Images require a format and file argument, plus optional caption and size
    # arguments; some of these might include spaces (and thus be surrounded by
    # double quotes).  Instead of unpacking all of this, we just cheat and use
    # the fact that Regina's usage of \image is always as a full-line command.
    s = re.sub(r'[\\@]image\s+\S.*\n', r'(Image available in HTML docs)\n\n', s)

    # If a \ref comes with a custom name, use it.  If not, see if the
    # tag is one that is known to us, and if not, just use the tag itself.
    s = re.sub(r'[\\@]ref\s+(\S+)\s+"([^"]+)"', r'\2', s)
    s = re.sub(r'[\\@]ref\s+sfsnotation(\s)',
               r'notation for Seifert fibred spaces\1', s)
    s = re.sub(r'[\\@]ref\s+pachner(\s)',
               r'Pachner moves on triangulations\1', s)
    s = re.sub(r'[\\@]ref\s*', r'', s)

    s = re.sub(r'[\\@]code\s?({\.[a-z]+}\s?)?(.*?)\s?[\\@]endcode',
               r"```\n\2\n```\n", s, flags=re.DOTALL)
    s = re.sub(r'[\\@]verbatim\s?(.*?)\s?[\\@]endverbatim',
               r"```\n\1\n```\n", s, flags=re.DOTALL)
    s = re.sub(r'[\\@]warning\s?(.*?)\s?\n\n',
               r'$.. warning::\n\n\1\n\n', s, flags=re.DOTALL)
    s = re.sub(r'[\\@]note\s?(.*?)\s?\n\n',
               r'$.. note::\n\n\1\n\n', s, flags=re.DOTALL)

    # Regina-specific paragraphs that we can ignore in Python:
    s = re.sub(r'\\headers\s(.*?)\s?\n\n', r'', s, flags=re.DOTALL)
    s = re.sub(r'\\headerfile\s(.*?)\s?\n\n', r'', s, flags=re.DOTALL)
    s = re.sub(r'\\cpp\s(.*?)\s?\n\n', r'', s, flags=re.DOTALL)
    s = re.sub(r'\\nocpp\s?(.*?)\s?\n\n', r'', s, flags=re.DOTALL)

    # Doxygen paragraphs that we will likewise ignore in Python:
    s = re.sub(r'[\\@]todo\s?(.*?)\s?\n\n', r'', s, flags=re.DOTALL)

    # Deprecated expects a version number for reST and not for Doxygen. Here the first word of the
    # doxygen directives is assumed to correspond to the version number
    s = re.sub(r'[\\@]deprecated\s(.*?)\s?(.*?)\s?\n\n',
               r'$.. deprecated:: \1\n\n\2\n\n', s, flags=re.DOTALL)
    s = re.sub(r'[\\@]since\s?(.*?)\s?\n\n',
               r'.. versionadded:: \1\n\n', s, flags=re.DOTALL)

    # Regina paragraphs that we need to translate and keep:
    s = re.sub(r'\\apinotfinal\s?(.*?)\s?\n\n',
        r'$.. warning::\n\nThe API for this class or function has not yet been finalised. This means that the interface may change in new versions of Regina, without maintaining backward compatibility. If you use this class directly in your own code, please check the detailed changelog with each new release to see if you need to make changes to your code.\n\n', s, flags=re.DOTALL)

    # HTML/TeX tags
    s = re.sub(r'<tt>(.*?)</tt>', r'``\1``', s, flags=re.DOTALL)
    s = re.sub(r'<pre>(.*?)</pre>', r"```\n\1\n```\n", s, flags=re.DOTALL)
    s = re.sub(r'<em>(.*?)</em>', r'*\1*', s, flags=re.DOTALL)
    s = re.sub(r'<i>(.*?)</i>', r'*\1*', s, flags=re.DOTALL)
    s = re.sub(r'<b>(.*?)</b>', r'**\1**', s, flags=re.DOTALL)
    s = re.sub(r'<sup>(.*?)</sup>', r'^{\1}', s, flags=re.DOTALL)
    s = re.sub(r'<sub>(.*?)</sub>', r'_{\1}', s, flags=re.DOTALL)
    s = re.sub(r'[\\@]f\$(.*?)[\\@]f\$', r':math:`\1`', s, flags=re.DOTALL)
    s = re.sub(r'<li>', r'\n\n* ', s)
    s = re.sub(r'</?ul>', r'', s)
    s = re.sub(r'</li>', r'\n\n', s)

    # Doxygen markdown support
    s = re.sub(r'([(,.+*/=^_\s-]|^)`([^`\s](?:[^`]*[^`\s])?)`((?:th)?[);,.?\s]|$)',
               r'\1``\2``\3', s, flags=re.DOTALL)

    # Special characters
    if not preserveAmpersands:
        s = re.sub(r'(^|[^\\])&lt;', r'\1<', s)
        s = re.sub(r'(^|[^\\])&gt;', r'\1>', s)
        s = re.sub(r'(^|[^\\])&le;', r'\1≤', s)
        s = re.sub(r'(^|[^\\])&ge;', r'\1≥', s)
        s = re.sub(r'(^|[^\\])&amp;', r'\1&', s)
        s = re.sub(r'(^|[^\\])&nbsp;', r'\1 ', s)
        s = re.sub(r'(^|[^\\])&pi;', r'\1π', s)
    s = re.sub(r'\\<', r'<', s)
    s = re.sub(r'\\>', r'>', s)
    s = re.sub(r'\\&', r'&', s)
    s = re.sub(r'\\%', r'%', s)

    # Regina's short Doxygen macros:
    s = re.sub(r'\\nullopt($|[^w])', r'``None``\1',s)

    s = s.replace('``true``', '``True``')
    s = s.replace('``false``', '``False``')
    s = s.replace('``null``', '``None``')
    s = s.replace('``nullptr``', '``None``')

    # Re-flow text
    wrapper = textwrap.TextWrapper()
    wrapper.expand_tabs = True
    wrapper.replace_whitespace = True
    wrapper.drop_whitespace = True
    wrapper.width = docstring_width
    wrapper.initial_indent = wrapper.subsequent_indent = ''
    last_indent = ''

    # TODO: In the following loop, any preformatted text *within* a list
    # will lose all indent information afterwards (i.e., it will effectively
    # end the list).  This should probably be fixed, though it does not yet
    # affect Regina's docs - at the time of writing, the only instance of
    # preformatted text within a list is in the Crossing class notes,
    # and here it happens at the end of each list item (so no problem).
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
            # Preformatted text could begin with whitespace that must be kept.
            # Only strip off the leading and trailing newlines that we added in
            # the regexes above, not all whitespace.
            preformatted = x.strip('\n')

            # However, we *do* indent this preformatted text to match
            # the (non-preformatted) text above it.  This means things
            # look correct if (for example) we have code blocks within a list.
            if last_indent:
                preformatted = last_indent + \
                    preformatted.replace('\n', '\n' + last_indent)

            result += preformatted
        else:
            # Split into paragraphs.
            for y in re.split(r'(?: *\n *){2,}', x):
                # Split out any list items in this paragraph.
                # A paragraph has optional plain text, followed by one or more
                # optional list items.  (In particular, the list items can
                # never be followed by additional plain text.)
                #
                # Note for later: doxygen supports also bullets of the
                # form "1.", "2.", etc.  This will require a bit more work
                # because we will need to override the automatic numbering.
                list_indents = []
                list_index = []
                for z in re.split(r'(?:^|\n)(\s*(?:[-+*]|-#)\s+)', y):
                    # The list bullets, including their initial indents, will
                    # appear as separate pieces of the split we just performed.
                    if len(z) == 0:
                        continue
                    zstrip = z.lstrip()
                    bullet = None
                    if len(zstrip) >= 2 and zstrip[:2] in [ '- ', '+ ', '* ' ]:
                        bullet = 0 # unnumbered
                    elif len(zstrip) >= 3 and zstrip[:3] == '-# ':
                        bullet = 1 # numbered

                    if bullet != None:
                        # This piece of the split is a bullet.
                        # Work out its indent and loop again to fetch
                        # the actual list item text.
                        indent = len(z) - len(zstrip)
                        if not list_indents:
                            list_indents = [ indent ]
                            list_index = [ 1 if bullet == 1 else None ]
                        elif list_indents[-1] < indent:
                            list_indents.append(indent)
                            list_index.append(1 if bullet == 1 else None)
                        else:
                            while list_indents and list_indents[-1] > indent:
                                list_indents.pop()
                            if list_index and (list_index[-1] != None):
                                list_index[-1] += 1
                            # TODO: Maybe it would be polite to check here
                            # whether our reduced indent matches a
                            # previously-seen indent level.
                            # TODO: We should also check when we repeat a
                            # prior indent that our numbered/unnumbered state
                            # matches.
                        continue

                    if list_indents:
                        wrapper.initial_indent = ' ' * sum( \
                            (2 if i == None else 3) for i in list_index[:-1])
                        wrapper.subsequent_indent = ' ' * sum( \
                            (2 if i == None else 3) for i in list_index)
                        # Sphinx wants us to render numbered lists with: #.
                        # Here we will actually use real numbers.
                        # For now we assume single-digit width.
                        if list_index[-1] == None:
                            z = '* ' + z
                        else:
                            z = str(list_index[-1]) + '. ' + z

                    wrapped = wrapper.fill(re.sub(r'\s+', ' ', z).strip())
                    last_indent = wrapper.subsequent_indent

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

    if node.raw_comment:
        if '\\nopython' in node.raw_comment:
            # The C++ docs tell us that this entity has no Python binding.
            return
        if '\\nodocstrings' in node.raw_comment:
            # Possibly this entity does have a Python binding, but the
            # C++ docs tell us not to generate docstrings for it.
            return

    if node.kind in RECURSE_LIST and \
            ((node.access_specifier not in ACCESS_BLACKLIST and \
                node.spelling not in CLASS_BLACKLIST and \
                node.spelling not in NAMESPACE_BLACKLIST) or \
            ((node.kind == CursorKind.CLASS_DECL or \
                node.kind == CursorKind.STRUCT_DECL) and \
                node.spelling in CLASS_WHITELIST)):
        if not (node.kind == CursorKind.NAMESPACE and \
                node.spelling in NAMESPACE_BLACKLIST):
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
            ((node.access_specifier not in ACCESS_BLACKLIST and \
                node.availability not in AVAILABILITY_BLACKLIST and \
                node.spelling not in MEMBER_BLACKLIST and \
                node.spelling not in CLASS_BLACKLIST and \
                (not node.is_move_constructor())) or \
            ((node.kind == CursorKind.CLASS_DECL or \
                node.kind == CursorKind.STRUCT_DECL) and \
                node.spelling in CLASS_WHITELIST)):
        sub_namespace = namespace
        if len(node.spelling) > 0:
            # We are seeing functions with inline definitions and/or
            # forward declarations appear multiple times in the output.
            # Try to ensure that their docstrings are listed only once.
            if node.canonical in printed:
                return
            if node.lexical_parent != node.semantic_parent and \
                    node != node.canonical:
                if node.kind in INLINE_DUPLICATES:
                    # This is probably an inline class method implementation
                    # (which may show up in the global namespace, not the
                    # class namespace, if the implementation happens outside
                    # the class declaration).
                    return
            if (node.kind == CursorKind.CLASS_DECL or \
                    node.kind == CursorKind.CLASS_TEMPLATE) and \
                    not node.is_definition():
                return

            # Unfortunately templated constructors do not show up as
            # constructors when we look at the corresponding CursorKind.
            if node.kind == CursorKind.CONSTRUCTOR or \
                    (node.kind == CursorKind.FUNCTION_TEMPLATE and \
                    (node.spelling == node.semantic_parent.spelling or
                    node.spelling.startswith(node.semantic_parent.spelling + \
                        '<'))):
                if node.is_copy_constructor():
                    name = '__copy'
                elif node.is_default_constructor():
                    name = '__default'
                else:
                    name = '__init'
            else:
                name = sanitize_name(d(node.spelling))

            fullname = 'regina::'
            if namespace:
                fullname = fullname + namespace + '::'
            fullname += name

            if node.raw_comment is None:
                # print('    Undocumented:', fullname, '-- skipping')
                return

            if node.spelling == 'operator<<':
                # We do not want docs for std::ostream output operators.
                # For now we skip *all* left shift operators; this may need to
                # become more nuanced at a later date.
                # print('    Left shift:', fullname, '-- skipping')
                return

            # Class template specialisations are a strange case.
            # Sometimes we want them in full (e.g., Face<dim, dim>);
            # sometimes we do not want the class docs but we want its members
            # (e.g., the triangulation alias classes), and sometimes we
            # do not want it at all (e.g., the ListView specialisations).
            #
            # For now:
            #
            # - Always take all the members, unless the class is marked
            #   \nodocstrings.  This is handled by the recursion code above.
            #
            # - Print the class docs only if we are not already printing docs
            #   for what appears to be the same class name in this same header.
            #
            if node.kind == CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION:
                for i in output:
                    if i[0] == sub_namespace and i[1] == name:
                        print('Skipping partial specialisation:',
                            node.displayname)
                        return

            # Note: xmlEncodeSpecialChars() includes a &...; special character
            # that needs to be left in this encoded form, since the raw encoding
            # is illustrated in the API docs.
            comment = d(node.raw_comment)
            comment = process_comment(comment,
                node.spelling == 'xmlEncodeSpecialChars')

            special = False
            if name == 'swap' and sub_namespace == '':
                # There are *so* many global swap(T&, T&) functions that
                # it will be helpful to name them according to the types
                # that they swap.  Otherwise their dostrings will all be called
                # regina::python::doc::swap, and there will be a risk of
                # inadvertently confusing one for another.
                children = [ c.type for c in node.get_children() \
                    if c.type.kind == TypeKind.LVALUEREFERENCE ]
                if len(children) == 2:
                    swapType = children[0].get_pointee().spelling
                    if swapType.startswith('regina::'):
                        swapType = swapType[8:]
                    pos = swapType.find('<')
                    if pos >= 0:
                        swapType = swapType[:pos]
                    if swapType:
                        output.append((swapType + '_', 'global_swap', \
                            filename, comment))
                        special = True

            if not special:
                output.append((sub_namespace, name, filename, comment))

            printed.append(node.canonical)


def _append_include_dir(args: list, include_dir: str, verbose: bool = True):
    """
    Add an include directory to an argument list (if it exists).
    """

    if os.path.isdir(include_dir):
        args.append(f"-I{shlex.quote(include_dir)}")
    elif verbose:
        print(f"Include directoy '{shlex.quote(include_dir)}' does not exist!")


def _append_definition(args: list, definition: str):
    """
    Add a compiler definition to an argument list.

    The definition is expected to be given in the format '<macro>=<value>',
    which will define <macro> to <value> (or 1 if <value> is omitted).
    """

    try:
        macro, value = definition.strip().split('=')
        macro = shlex.quote(macro.strip())
        value = shlex.quote(value.strip()) if value else '1'

        args.append(f"-D{macro}={value}")
    except ValueError as exc:
        # most likely means there was no '=' given
        # check if argument is valid identifier
        if re.search(r'^[A-Za-z_][A-Za-z0-9_]*', definition):
            args.append(f"-D{definition}")
        else:
            print(f"Failed to parse definition: {shlex.quote(definition)}")
    except:
        print(f"Failed to parse definition: {shlex.quote(definition)}")


def main():
    """
    Entry point for the `mkdoc` console script.

    Parses the  commandline arguments given to the console script and passes them on to `mkdoc`.
    """

    parser = argparse.ArgumentParser(
            prog='mkdoc',
            description="Processes a sequence of C/C++ headers and extracts comments for use in pybind11 binding code.",
            epilog="(Other compiler flags that Clang understands can also be supplied)",
            allow_abbrev=False)

    parser.add_argument("-v", "--version", action="version", version=f"%(prog)s {__version__}")

    parser.add_argument("-o", "--output", action="store", type=str, dest="output", metavar="<file>",
                        help="Write to the specified file (default: use stdout).")

    parser.add_argument("-w", "--width", action="store", type=int, dest="width", metavar="<width>",
                        help="Specify docstring width before wrapping.")

    parser.add_argument("-I", action="append", type=str, dest="include_dirs", metavar="<dir>",
                        help="Specify an directory to add to the list of include search paths.")

    parser.add_argument("-D", action="append", type=str, metavar="<macro>=<value>", dest="definitions",
                        help="Specify a compiler definition, i.e. define <macro> to <value> (or 1 if <value> omitted).")

    parser.add_argument("header", type=str, nargs='+', help="A header file to process.")

    [parsed_args, unparsed_args] = parser.parse_known_args()

    mkdoc_args = []
    mkdoc_out = parsed_args.output
    docstring_width = parsed_args.width

    if parsed_args.include_dirs is not None:
        for include_dir in parsed_args.include_dirs:
            _append_include_dir(mkdoc_args, include_dir)

    if parsed_args.definitions is not None:
        for definition in parsed_args.definitions:
            _append_definition(mkdoc_args, definition)

    for arg in unparsed_args:
        if arg.startswith("-I"):
            _append_include_dir(mkdoc_args, arg[2:])
        elif arg.startswith("-D"):
            _append_definition(mkdoc_args, arg[2:])
        else:
            # append argument as is and hope for the best
            mkdoc_args.append(shlex.quote(arg))

    for header in parsed_args.header:
        mkdoc_args.append(shlex.quote(header))

    mkdoc(mkdoc_args, docstring_width, mkdoc_out)

    return 0
def read_args(args):
    parameters = []
    filenames = []
    if "-x" not in args:
        parameters.extend(['-x', 'c++'])
    if not any(it.startswith("-std=") for it in args):
        parameters.append('-std=c++11')
    parameters.append('-Wno-pragma-once-outside-header')
    parameters.append('-D__DOCSTRINGS')
    parameters.append('-D__APIDOCS')

    # Sometimes header X includes Y which includes X, and we would like
    # to pick up comments from this re-included copy of X.  (See
    # engine/maths/spec/perm*.h for examples of this.)
    parameters.append('-fretain-comments-from-system-headers')

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

    global errors_detected, inline
    for filename in filenames:
        inline = (filename in INLINE_FILES)
        if not inline:
            for d in INLINE_DIRS:
                if filename.startswith(d + '/'):
                    inline = True
                    break
        if inline:
            print('Processing "%s" (inline) ..' % filename, file=sys.stderr)
        else:
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
  Do not edit! They were automatically extracted by ../gendoc.sh.
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
        if inline:
            print('constexpr const char *%s =%sR"doc(%s)doc";' %
                  (name, '\n' if '\n' in comment else ' ', comment), \
                  file=out_file)
        else:
            print('static const char *%s =%sR"doc(%s)doc";' %
                  (name, '\n' if '\n' in comment else ' ', comment), \
                  file=out_file)

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


main()
