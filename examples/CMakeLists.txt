
SET(examplesdir ${PKGDATADIR}/examples)

SET(examples_files README.txt closed-hyp-census.rga closed-or-census.rga closed-or-census-large.rga closed-nor-census.rga knot-link-census.rga sample-misc.rga sig-3mfd-census.rga sig-prime-min-census.rga snappea-census.rga progress.py progress.session tree.py tree.session triangulation.py triangulation.session)
INSTALL(FILES ${examples_files} DESTINATION ${examplesdir})

