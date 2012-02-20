# TODO: Install application icons for KDE/GNOME

set (IMG_FILES )

set (IMGS barycentric cone connsum csvexport dehydrated doublecover filter_comb filter_prop finite orb orient packet_angles packet_container packet_filter packet_pdf packet_script packet_surfaces packet_text packet_triangulation packet_view patience python_console regina snappea)
set (SIZES 16 22 32 48 64 128)

foreach (IMG_CLASS ${IMGS})
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
      SET (IMG_FILES ${IMG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
  endforeach (IMG_SIZE ${SIZES})
endforeach (IMG_CLASS ${IMGS})

INSTALL (FILES ${IMG_FILES} SOURCES DESTINATION ${PKGDATADIR}/icons/regina)

ADD_SUBDIRECTORY(oxygen)
