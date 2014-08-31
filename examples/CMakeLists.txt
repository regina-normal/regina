
SET(examplesdir ${PKGDATADIR}/examples)

SET(examples_files
  README.txt
  closed-hyp-census.rga
  closed-or-census.rga
  closed-nor-census.rga
  cusped-hyp-or-census.rga
  cusped-hyp-nor-census.rga
  hyp-knot-link-census.rga
  sample-misc.rga
  progress.py
  progress.session
  tree.py
  tree.session
  triangulation.py
  triangulation.session
  )
INSTALL(FILES ${examples_files} DESTINATION ${examplesdir})

