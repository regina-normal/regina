
# TODO: The doxygen files generated in the build tree are not removed
# by "make clean".

ADD_CUSTOM_COMMAND(OUTPUT index.html
  COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/engine/doxygen/docs.conf
  COMMAND cp `find "${PROJECT_SOURCE_DIR}/engine" -name '*.png'` .
  )

ADD_CUSTOM_TARGET(docs ALL DEPENDS index.html)

INSTALL(
  DIRECTORY ${PROJECT_BINARY_DIR}/docs/engine/
  DESTINATION ${PKGDATADIR}/engine-docs
  FILES_MATCHING
  PATTERN CMakeFiles EXCLUDE
  PATTERN "*.css"
  PATTERN "*.dot"
  PATTERN "*.gif"
  PATTERN "*.html"
  PATTERN "*.jpg"
  PATTERN "*.png"
  PATTERN "*.txt"
  )

