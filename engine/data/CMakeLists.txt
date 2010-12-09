
set(INTERNALDATADIR $(PKGDATADIR)/data)

set(internaldata_DATA README.txt snappea-census-sec5.dat snappea-census-sec6o.dat snappea-census-sec6n.dat snappea-census-sec7o.dat snappea-census-sec7n.dat)

extra_dist(FILES create-snappea-census-data.py verify-snappea-census-data.py )

install(FILES internaldata_DATA DESTINATION ${PKGDATADIR}/data)

