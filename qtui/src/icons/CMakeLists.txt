set (IMG_FILES )

set (IMGS barycentric cone connsum csvexport dehydrated doublecover filter_comb filter_prop finite orb orient packet_angles packet_container packet_filter packet_pdf packet_script packet_surfaces packet_text packet_triangulation packet_view python_console regina snappea)
set (SIZES 16 22 32 48 64 128)

foreach (IMG_CLASS ${IMGS})
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
      SET (IMG_FILES ${IMG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
  endforeach (IMG_SIZE ${SIZES})
endforeach (IMG_CLASS ${IMGS})

INSTALL (FILES ${IMG_FILES} SOURCES DESTINATION ${PKGDATADIR}/icons/regina)

if (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # Install application icons for freedesktop.org.
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/regina-${IMG_SIZE}.png)
      INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/regina-${IMG_SIZE}.png DESTINATION ${ICONDIR}/hicolor/${IMG_SIZE}x${IMG_SIZE}/apps RENAME regina.png)
      INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/x-regina-${IMG_SIZE}.png DESTINATION ${ICONDIR}/hicolor/${IMG_SIZE}x${IMG_SIZE}/mimetypes RENAME application-x-regina.png)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/regina-${IMG_SIZE}.png)
  endforeach (IMG_SIZE ${SIZES})
endif (${REGINA_INSTALL_TYPE} STREQUAL XDG)

ADD_SUBDIRECTORY(oxygen)
