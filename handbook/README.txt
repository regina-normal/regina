User handbook and file format reference
---------------------------------------

This directory is used to generate the following user documentation:

  regina/ - User handbook
  regina-xml/ - XML file format reference

This documentation is written in KDE's XML docbook format.  It should be
converted to HTML automatically as part of the build process, assuming that
cmake was able to locate a working xsltproc.

Regarding ksgmltools/:

  The directory ksgmltools2/ contains a stripped-down version of the KDE
  docbook files so that we can build the documentation using just xsltproc,
  without a full KDE installation and without KDE-specific tools such as
  meinproc).  These files were taken from kdelibs 4.4.1.  See
  ksgmltools2/LICENSE and ksgmltools2/docbook/xsl/README for licensing.

  All localisation/translation support has been removed from this copy,
  in order to keep the copy of ksgmltools2/ as small as possible.  If
  you wish to translate these documents, please mail the Regina
  developers and we can re-add the supporting files for your language.

 -- Ben Burton <bab@debian.org>, Thu, 11 Jun 2026 01:02:36 +1000
