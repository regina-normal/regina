
ADD_CUSTOM_COMMAND(OUTPUT index.html
  COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/engine/doxygen/docs.conf
  COMMAND cp `find "${PROJECT_SOURCE_DIR}/engine" -name '*.png'` .
  )

ADD_CUSTOM_TARGET(docs ALL DEPENDS index.html)

# TODO: Install the engine docs.

# TODO: Fix this.
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "*.html *.png *.gif *.css *.dot")

# extensions = css dot gif html jpg png txt
# pkgdocdir = $(pkgdatadir)/engine-docs

#	$(mkinstalldirs) "$(DESTDIR)$(pkgdocdir)"
#	for i in $(extensions); do \
#		for j in `find . -name "*.$$i"`; do \
#			$(INSTALL_DATA) $$j "$(DESTDIR)$(pkgdocdir)"; \
#		done; \
#	done

