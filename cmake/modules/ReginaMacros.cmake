# CMake macros specific to Regina.
#
# Copyright (c) Ben Burton, 2012-2016
# Licensed under the GNU General Public License, version 2 or later
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.


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
    add_custom_command(OUTPUT ${_doc}
      COMMAND ${XSLTPROC_EXECUTABLE} --path ${_dtd} -o ${CMAKE_CURRENT_BINARY_DIR}/ ${_ssheet} ${_input}
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

