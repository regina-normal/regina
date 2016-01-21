set (IMG_FILES )

set (IMGS
  barycentric
  canonical
  components
  cone
  connectedsum
  connectedsumwith
  csvexport
  dehydrated
  delete
  doublecover
  drilledge
  fill
  filter_comb
  filter_prop
  finite
  insert
  locked
  orb
  orient
  packet_angles
  packet_container
  packet_dim2triangulation
  packet_dim4triangulation
  packet_filter
  packet_pdf
  packet_script
  packet_snappea
  packet_surfaces
  packet_text
  packet_triangulation
  packet_view
  puncture
  randomise
  regina
  signature-2d
  signature-3d
  simplify
  snappea
  vtxlinks
  )
set (SIZES 16 16@2x 22 22@2x 32 32@2x 48 48@2x 64 64@2x)

foreach (IMG_CLASS ${IMGS})
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
      SET (IMG_FILES ${IMG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
  endforeach (IMG_SIZE ${SIZES})
endforeach (IMG_CLASS ${IMGS})

INSTALL (FILES ${IMG_FILES} SOURCES DESTINATION ${PKGDATADIR}/icons)

ADD_SUBDIRECTORY(oxygen)
