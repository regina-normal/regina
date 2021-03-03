# CMake macros specific to Regina.
#
# Copyright (c) Ben Burton, 2012-2021
# Licensed under the GNU General Public License, version 2 or later
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.


# Macro: REGINA_ESCAPE_BASH(output input)
#
# Sets the variable ${output} to be a variant of ${input} suitable for
# use in a bash script between single quotes.
#
# Both output and input should be variable names.
#
macro (REGINA_ESCAPE_BASH _output _input)
  string(REPLACE "'" "'\"'\"'" ${_output} "${${_input}}")
endmacro (REGINA_ESCAPE_BASH)


# Macro: REGINA_ESCAPE_PERL(output input)
#
# Sets the variable ${output} to be a variant of ${input} suitable for
# use in a perl script between single quotes.
#
# Both output and input should be variable names.
#
macro (REGINA_ESCAPE_PERL _output _input)
  string(REPLACE "\\" "\\\\" ${_output} "${${_output}}")
  string(REPLACE "'" "\\'" ${_output} "${${_input}}")
endmacro (REGINA_ESCAPE_PERL)


# Macro: REGINA_CREATE_HANDBOOK(lang)
#
# Builds an HTML manual from XML Docbook sources.
#
# Includes code from KDE4_CREATE_HANDBOOK from the KDE cmake scripts.
# The KDE cmake scripts are licensed as follows:
#
# Copyright (c) 2006-2009 Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2006, 2007, Laurent Montel, <montel@kde.org>
# Copyright (c) 2007 Matthias Kretz <kretz@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
macro (REGINA_CREATE_HANDBOOK _lang)
  # Build the docbook documentation.

  set(_doc ${CMAKE_CURRENT_BINARY_DIR}/index.html)

  # The source variables below are relative to ${CMAKE_CURRENT_SOURCE_DIR}.
  # We use relative paths here because xsltproc on Windows seems to have
  # trouble with the format that cmake uses for absolute paths.
  #
  set(_input index.docbook)
  set(_custom ../ksgmltools2/customization)
  set(_dtd ${_custom}/dtd)
  set(_ssheet ${_custom}/regina.xsl)

  get_filename_component(_handbook ${CMAKE_CURRENT_SOURCE_DIR} NAME)

  file(GLOB _docs *.docbook)
  if (REGINA_DOCS)
    add_custom_command(OUTPUT ${_doc}
      COMMAND ${UNZIP_EXECUTABLE} -o -j -d ${CMAKE_CURRENT_BINARY_DIR}
        ${REGINA_DOCS_FILE} "docs/${_lang}/${_handbook}/\\*")
  else (REGINA_DOCS)
    # xsltproc requires an URI-encoded output directory.
    #
    # The following block of code tries to use cmake to URI-encode the
    # output directory ${CMAKE_CURRENT_BINARY_DIR}.
    #
    # This code still breaks if the directory name includes a backslash,
    # but probably a pile of other code breaks in similar situations, due
    # to cmake's argument parsing.

    set(_output "")
    string(REGEX MATCHALL . chars ${CMAKE_CURRENT_BINARY_DIR})
    foreach(c ${chars})
      # This code percent-encodes [ and ], which is unnecessary but harmless.
      # If someone knows how to include [ and ] inside cmake's regex [...]
      # construct then *please* let me know.  I couldn't do it.
      string(REGEX MATCH "^[A-Za-z0-9_.~!#$&'()*+,/:;=?@-]$" m "${c}")
      if (NOT m STREQUAL "")
        # These characters are allowed in URIs.
        string(APPEND _output "${c}")
      else (NOT m STREQUAL "")
        # All other characters must be percent-encoded.
        # Note that these may be multi-byte unicode characters.
        #
        # Whilst CMake 3.18 introduces hex encoding for strings,
        # we must preserve compatibility with older cmake, and so we
        # use file read/writes to get the hex encoding.  Again I'd be
        # super happy for a better way of doing this.
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/_hex.dat "${c}")
        file(READ ${CMAKE_CURRENT_BINARY_DIR}/_hex.dat _hexraw HEX)

        string(REGEX MATCHALL "(..)" _hexlist "${_hexraw}")
        list(JOIN _hexlist "%" _hex)
        string(PREPEND _hex "%")
        string(TOUPPER "${_hex}" _hexupper)
        string(APPEND _output "${_hexupper}")
      endif (NOT m STREQUAL "")
    endforeach()

    add_custom_command(OUTPUT ${_doc} VERBATIM
      COMMAND ${XSLTPROC_EXECUTABLE} --path ${_dtd} -o "${_output}/" ${_ssheet} "${_input}"
      DEPENDS ${_docs} ${_ssheet}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif (REGINA_DOCS)
  add_custom_target(${_handbook}-html ALL DEPENDS ${_doc})

  file(GLOB _support *.png *.css *.html)
  install(FILES ${_support} DESTINATION ${HTMLDIR}/${_lang}/${_handbook})
  install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DESTINATION ${HTMLDIR}/${_lang}
    FILES_MATCHING
    PATTERN CMakeFiles EXCLUDE
    PATTERN "*.html"
    )
endmacro (REGINA_CREATE_HANDBOOK)

