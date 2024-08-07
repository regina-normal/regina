set (IMG_FILES )

set (IMGS
  alternating
  barycentric
  boundaries
  canonical
  components
  cone
  connectedsum
  connectedsumwith
  crossing-l
  crossing-u
  crossing+l
  crossing+u
  csvexport
  dehydrated
  delete
  disjointunion
  doublecover
  drilledge
  examples
  fill
  filter_comb
  filter_prop
  finite
  insert
  lock
  orb
  orient
  packet_angles
  packet_attachment
  packet_container
  packet_filter
  packet_hypersurfaces
  packet_link
  packet_pdf
  packet_script
  packet_snappea
  packet_spatiallink
  packet_surfaces
  packet_text
  packet_triangulation2
  packet_triangulation3
  packet_triangulation4
  packet_triangulation5
  packet_triangulation6
  packet_triangulation7
  packet_triangulation8
  packet_triangulation9
  packet_triangulation10
  packet_triangulation11
  packet_triangulation12
  packet_triangulation13
  packet_triangulation14
  packet_triangulation15
  packet_view
  parallel
  puncture
  randomise
  refine
  reflect
  regina
  reverse
  rotate
  run
  selfframe
  signature-2d
  signature-3d
  signature-4d
  signature-knot
  simplify
  simplify-link
  snappea
  thicker
  thinner
  troubleshoot
  vtxlinks
  welcome
  )
set (SIZES 16 16@2x 22 22@2x 32 32@2x 48 48@2x 64 64@2x)

foreach (IMG_CLASS ${IMGS})
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png")
      SET (IMG_FILES ${IMG_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png")
    endif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png")
  endforeach (IMG_SIZE ${SIZES})
endforeach (IMG_CLASS ${IMGS})

INSTALL (FILES ${IMG_FILES} SOURCES DESTINATION "${PKGDATADIR}/icons")

ADD_SUBDIRECTORY(system)
