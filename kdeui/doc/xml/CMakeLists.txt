# Make an HTML version of the file format documentation.
SET(WEB_SS ${KDE4_DATA_INSTALL_DIR}/ksgmltools2/customization/kde-web.xsl)
ADD_CUSTOM_COMMAND(OUTPUT index.html
  COMMAND ${KDE4_MEINPROC_EXECUTABLE} --check --stylesheet=${WEB_SS} ${CMAKE_CURRENT_SOURCE_DIR}/index.docbook
  COMMAND ${CMAKE_SOURCE_DIR}/admin/htmlfix *.html
  DEPENDS index.docbook
  )
ADD_CUSTOM_TARGET(xmlref ALL DEPENDS index.html)
INSTALL(
  DIRECTORY ${PROJECT_BINARY_DIR}/kdeui/doc/regina-xml/
  DESTINATION ${HTML_INSTALL_DIR}/en/regina-xml
  FILES_MATCHING PATTERN "*.html"
  )
