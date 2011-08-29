kde4_create_handbook(index.docbook INSTALL_DESTINATION ${HTML_INSTALL_DIR}/en)

# Also make an HTML version of the handbook that is suitable for any
# web browser.  This allows us to avoid a dependency on kdebase.
SET(WEB_SS ${KDE4_DATA_INSTALL_DIR}/ksgmltools2/customization/kde-web.xsl)
ADD_CUSTOM_COMMAND(OUTPUT index.html
  COMMAND ${KDE4_MEINPROC_EXECUTABLE} --check --stylesheet=${WEB_SS} ${CMAKE_CURRENT_SOURCE_DIR}/index.docbook
  COMMAND ${CMAKE_SOURCE_DIR}/admin/htmlfix *.html
  )
ADD_CUSTOM_TARGET(html ALL DEPENDS index.html)
INSTALL(
  DIRECTORY ${PROJECT_BINARY_DIR}/kdeui/doc/regina/
  DESTINATION ${HTML_INSTALL_DIR}/en/regina
  FILES_MATCHING PATTERN "*.html"
  )
