# TODO: Install application icons for KDE/GNOME

set (IMG_FILES )

set (IMGS patience regina)
set (SIZES 16 22 32 48 64 128)

foreach (IMG_CLASS ${IMGS})
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
      SET (IMG_FILES ${IMG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
  endforeach (IMG_SIZE ${SIZES})
endforeach (IMG_CLASS ${IMGS})

INSTALL (FILES ${IMG_FILES} DESTINATION ${PKGDATADIR}/icons/regina)
