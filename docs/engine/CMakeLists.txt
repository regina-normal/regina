
# TODO: The doxygen files generated in the build tree are not removed
# by "make clean".

if (REGINA_ENGINE_DOCS)
  ADD_CUSTOM_COMMAND(OUTPUT index.html VERBATIM
    COMMAND "${UNZIP_EXECUTABLE}" -o -j -d "${CMAKE_CURRENT_BINARY_DIR}"
      "${REGINA_ENGINE_DOCS_FILE}")
else (REGINA_ENGINE_DOCS)
  FILE(GLOB _png ../../engine/*/*.png)
  FILE(COPY ${_png} DESTINATION .)
  ADD_CUSTOM_COMMAND(OUTPUT index.html VERBATIM
    COMMAND "${DOXYGEN_EXECUTABLE}" "${PROJECT_BINARY_DIR}/engine/doxygen/docs.conf"
    )
endif (REGINA_ENGINE_DOCS)

ADD_CUSTOM_TARGET(docs ALL DEPENDS index.html)

INSTALL(
  DIRECTORY "${PROJECT_BINARY_DIR}/docs/engine/"
  DESTINATION "${PKGDATADIR}/engine-docs"
  FILES_MATCHING
  PATTERN CMakeFiles EXCLUDE
  PATTERN "*.css"
  PATTERN "*.dot"
  PATTERN "*.gif"
  PATTERN "*.html"
  PATTERN "*.jpg"
  PATTERN "*.js"
  PATTERN "*.png"
  PATTERN "*.txt"
  )

