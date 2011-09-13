#!/usr/bin/perl -w

###############################################################################
# Sanity checks CMakeLists.txt files.                                         #
# Copyright (C) 2006-2009 by Allen Winter <winter@kde.org>                    #
# Copyright (C) 2008-2010 by Laurent Montel <montel@kde.org>                  #
#                                                                             #
# This program is free software; you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by        #
# the Free Software Foundation; either version 2 of the License, or           #
# (at your option) any later version.                                         #
#                                                                             #
# This program is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU General Public License along     #
# with this program; if not, write to the Free Software Foundation, Inc.,     #
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.               #
#                                                                             #
###############################################################################
#
# A program to check KDE CMakeLists.txt files for common errors.
#

# This program needs a rewrite.. it assumes that each CMake command is
# all on 1 line.  So stuff can easily fall through the cracks -- Allen

# Program options:
#   --help:          display help message and exit
#   --version:       display version information and exit
#

use strict;
use Getopt::Long;
use Cwd 'abs_path';

my($Prog) = 'cmakelint.pl';
my($Version) = '1.10';

my($help) = '';
my($version) = '';
my($verbose) = '';

exit 1
if (!GetOptions('help' => \$help, 'version' => \$version, 'verbose' => \$verbose));

&Help() if ($help);
if ($#ARGV < 0){ &Help(); exit 0; }
&Version() if ($version);

my($f,$tot_issues);
$tot_issues=0;
for $f (@ARGV) {
  $tot_issues += &processFile($f);
}
exit $tot_issues;

sub processFile() {
  my($in) = @_;
  print "Processing $in:\n";
  open(IN,"$in") || die "Couldn't open $in";

  my($apath) = abs_path($in);
  my($in_kdelibs)=0;
  $in_kdelibs=1 if ($apath =~ m+/kdelibs/+);
  my($in_kdepimlibs)=0;
  $in_kdepimlibs=1 if ($apath =~ m+/kdepimlibs/+);
  my($in_kdebase)=0;
  $in_kdebase=1 if ($apath =~ m+/kdebase/+);
  my($in_kdegames)=0;
  $in_kdegames=1 if ($apath =~ m+/kdegames/+);

  my($top_of_module)=0;
  $top_of_module=1 if ($apath =~ m+/koffice/CMakeLists.txt+);
  $top_of_module=1 if ($apath =~ m+/kdereview/CMakeLists.txt+);
  $top_of_module=1 if ($apath =~ m+/playground/[a-zA-Z_1-9]*/CMakeLists.txt+);
  $top_of_module=1 if ($apath =~ m+/extragear/[a-zA-Z_1-9]*/CMakeLists.txt+);
  $top_of_module=1 if ($apath =~ m+/kdebase/(apps|runtime|workspace)/CMakeLists.txt+);
  $top_of_module=1 if ($apath =~ m+/kde(libs|pimlibs|base|base-apps|base-runtime|base-workspace|accessibility|addons|admin|artwork|bindings|edu|games|graphics|multimedia|network|pim|sdk|toys|utils|develop|devplatform|webdev|plasma-addons)/CMakeLists.txt+);

  my($top_of_project)=0;
  $top_of_project=1 if ($apath =~ m+/koffice/[a-zA-Z_1-9]*/CMakeLists.txt+);
  $top_of_project=1 if ($apath =~ m+/kdereview/[a-zA-Z_1-9]*/CMakeLists.txt+);
  $top_of_project=1 if ($apath =~ m+/playground/[a-zA-Z_1-9]*/[a-zA-Z_1-9]*/CMakeLists.txt+);
  $top_of_project=1 if ($apath =~ m+/extragear/[a-zA-Z_1-9]*/[a-zA-Z_1-9]*/CMakeLists.txt+);
  $top_of_project=1 if ($apath =~ m+/kde(libs|pimlibs|base|accessibility|addons|admin|artwork|bindings|edu|games|graphics|multimedia|network|pim|sdk|toys|utils|develop|devplatform|webdev|plasma-addons)/[a-zA-Z_1-9]*/CMakeLists.txt+);
  $top_of_project=0 if ($apath =~ m+/(cmake|pics)/+);

  my(@lines) = <IN>;
  my($line,$pline);
  my($linecnt)=0;
  my($issues)=0;
  my(@ch,$c);
  my($nob,$ncb)=(0,0);
  my($nop,$ncp)=(0,0);
  my($pack,%optpacks);

  #look for "bad" stuff
  my($prevline)="";
  foreach $line (@lines) {
    $linecnt++;
    chomp($line);
    #pline is used for paren/brace matching only
    $pline = $line;
    $pline =~ s/".*"//g;
    $pline =~ s/#.*$//;

    $line =~ s/#.*$//; #remove comments

    next if (! $line);                   #skip empty lines
    next if ($line =~ m/^[[:space:]]$/); #skip blank lines

    @ch = split(//,$pline);
    $nob = $ncb = 0;
    foreach $c (@ch) {
      $nop++ if ($c eq '(');
      $ncp++ if ($c eq ')');
      $nob++ if ($c eq '{');
      $ncb++ if ($c eq '}');
    }

    if ($in !~ m+kjsembed/qtonly/CMakeLists.txt+) {
      if ($line =~ m/macro_optional_find_package\s*\(\s*([a-zA-Z0-9]*).*\)/i ||
	  $line =~ m/find_package\s*\(\s*([a-zA-Z0-9]*).*\)/i ||
	  $line =~ m/find_program\s*\(\s*([a-zA-Z0-9_]*).*\)/i) {
	$pack = lc($1);
	$pack = "libusb" if ($pack eq "usb");
	$pack = "mysql_embedded" if ($pack eq "mysql");
	if ($pack !~ m/^(carbon|iokit|qt4|kde4internal|kde4|kdewin32|kdepimlibs|kdevplatform|gpgme|kdcraw|kexiv2)/) {
	  $optpacks{$pack}{'name'} = $pack;
	  $optpacks{$pack}{'log'} = 0;
	}
      }
      if ($line =~ m/macro_log_feature\(\s*([A-Z0-9_]*).*\)/i) {
	$pack = lc($1);
	if ($pack !~ m/^(compositing|strigiqtdbusclient|xsltproc|qt_qtopengl|ggzconfig)_/ &&
	    $pack !~ m/x11_.*_found/ &&
	    $pack !~ m/true/i && $pack !~ m/false/i) {
	  $pack =~ s/_xcb//;
	  $pack =~ s/have_//;
	  $pack =~ s/_found//;
	  $pack =~ s/_video//;
	  $pack =~ s/shared_mime_info/sharedmimeinfo/;
	  if (!defined($optpacks{$pack}{'name'})) {
#	    if ($pack !~ m/^(kwin_compositing|current_alsa)/) {
	      $issues++;
	      &printIssue($line,$linecnt,"macro_log_feature($pack) used without a corresponding find_package");
	    }
#	  }
	  $optpacks{$pack}{'log'} = 1;
	}
      }
    }

    $issues += &checkLine($line,$linecnt,
			  '[[:space:]]{\$',
			  'replace "{$" with "${", or garbage detected');

    $issues += &checkLine($line,$linecnt,
			  '[[^:print:]]{\$',
			  'non-printable characters detected');

    $issues += &checkLine($line,$linecnt,
			  '[Kk][Dd][Ee]4_[Aa][Uu][Tt][Oo][Mm][Oo][Cc]',
			  'KDE4_AUTOMOC() is obsolete. Remove it.');

    $issues += &checkLine($line,$linecnt,
                          '^[[:space:]]*[Qq][Tt]4_[Aa][Uu][Tt][Oo][Mm][Oo][Cc]',
                          'No need for QT4_AUTOMOC(). Remove it.');

    #$issues += &checkLine($line,$linecnt,
    #			  '[Kk][Dd][Ee]3_[Aa][Dd][Dd]_[Kk][Pp][Aa][Rr][Tt]',
    #			  'Use KDE4_ADD_PLUGIN() instead of KDE3_ADD_KPART()');

    $issues += &checkLine($line,$linecnt,
			  '^[[:space:]]*[Aa][Dd][Dd]_[Ll][Ii][Bb][Rr][Aa][Rr][Yy]',
			  'Use KDE4_ADD_LIBRARY() instead of ADD_LIBRARY()');

    $issues += &checkLine($line,$linecnt,
                          'DESTINATION[[:space:]]\${APPLNK_INSTALL_DIR}',
                          'APPLNK_INSTALL_DIR is dead with kde4 replace "${APPLNK_INSTALL_DIR}" with "${XDG_APPS_INSTALL_DIR}" and convert desktop file to xdg format (add Categories)');

    $issues += &checkLine($line,$linecnt,
                          'DESTINATION[[:space:]]\${MIME_INSTALL_DIR}',
                          'Files installed into MIME_INSTALL_DIR will not read. Port them on freedesktop xdg mimetypes.');

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]/lib/kde[[:digit:]]',
			  'replace /lib/kde" with "${PLUGIN_INSTALL_DIR}"');

    if ($line !~ m/kdeinit/ && $prevline !~ m/kdeinit/) {
      $issues += &checkLine($line,$linecnt,
			    'DESTINATION[[:space:]]\$\{LIB_INSTALL_DIR\}\s*\)',
			    'replace "DESTINATION ${LIB_INSTALL_DIR}" with "${INSTALL_TARGETS_DEFAULT_ARGS}"');
      $issues += &checkLine($line,$linecnt,
			    'DESTINATION[[:space:]]lib',
			    'replace "DESTINATION lib" with "${INSTALL_TARGETS_DEFAULT_ARGS}"');
    }

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]\${LIB_INSTALL_DIR}/\$',
			  'replace "${LIB_INSTALL_DIR}/${...}" with "${LIB_INSTALL_DIR}/realname"');

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]/*include/*',
			  'replace "include" or "/include" with "${INCLUDE_INSTALL_DIR}"');
    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]\${INCLUDE_INSTALL_DIR}/\$',
			  'replace "${INCLUDE_INSTALL_DIR}/${...}" with "${INCLUDE_INSTALL_DIR}/realname"');

    if ($line !~ m/PROGRAMS/ && $line !~ m/FILES/ &&
	$line !~ m/PERMISSIONS/ &&
	$line !~ m/OWNER/ && $line !~ m/GROUP/ && $line !~ m/WORLD/) {
      $issues += &checkLine($line,$linecnt,
			    'DESTINATION[[:space:]]\$\{BIN_INSTALL_DIR\}\s*\)',
			    'replace "DESTINATION ${BIN_INSTALL_DIR}" with "${INSTALL_TARGETS_DEFAULT_ARGS}"');

      $issues += &checkLine($line,$linecnt,
			    'DESTINATION[[:space:]]/*bin/*',
			    'replace "DESTINATION bin" or "/bin" with "${INSTALL_TARGETS_DEFAULT_ARGS}"');
    }

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]\${BIN_INSTALL_DIR}/\$',
			  'replace "${BIN_INSTALL_DIR}/${...}" with "${BIN_INSTALL_DIR}/realname"');

    $issues += &checkLine($line,$linecnt,
                          'DESTINATION[[:space:]]/*share/doc/HTML/*',
                          'replace "share/doc/HTML/" or "/share/doc/HTML/" with "${HTML_INSTALL_DIR}"');

    $issues += &checkLine($line,$linecnt,
 			  'DESTINATION[[:space:]]/*share/apps/*',
 			  'replace "share/apps" or "/share/apps" with "${DATA_INSTALL_DIR}"');
    $issues += &checkLine($line,$linecnt,
 			  'DESTINATION[[:space:]]\${DATA_INSTALL_DIR}/\$',
 			  'replace "${DATA_INSTALL_DIR}/${...}" with "${DATA_INSTALL_DIR}/realname"');

    $issues += &checkLine($line,$linecnt,
 			  'DESTINATION[[:space:]]/*share/applications/*',
 			  'replace "share/applications" or "/share/applications" with "${XDG_APPS_INSTALL_DIR}"');

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]/*share/autostart/*',
			  'replace "share/autostart" or "/share/autostart" with "${AUTOSTART_INSTALL_DIR}"');
    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]\${AUTOSTART_INSTALL_DIR}/\$',
			  'replace "${AUTOSTART_INSTALL_DIR}/${...}" with "${AUTOSTART_INSTALL_DIR}/realname"');

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]/*share/icons/*',
			  'replace "share/icons" or "/share/icons" with "${ICON_INSTALL_DIR}"');
    if ($in !~ m/IconThemes/) {
      $issues += &checkLine($line,$linecnt,
			    'DESTINATION[[:space:]]\${ICON_INSTALL_DIR}/\$',
			    'replace "${ICON_INSTALL_DIR}/${...}" with "${ICON_INSTALL_DIR}/realname"');
    }
    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]/*share/locale/*',
			  'replace "share/locale" or "/share/locale" with "${LOCALE_INSTALL_DIR}"');
    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]\${LOCALE_INSTALL_DIR}/\$',
			  'replace "${LOCALE_INSTALL_DIR}/${...}" with "${LOCALE_INSTALL_DIR}/realname"');

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]/*share/services/*',
			  'replace "share/services" or "/share/services" with "${SERVICES_INSTALL_DIR}"');
    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]\${SERVICES_INSTALL_DIR}/\$',
			  'replace "${SERVICES_INSTALL_DIR}/${...}" with "${SERVICES_INSTALL_DIR}/realname"');

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]/*share/sounds/*',
			  'replace "share/sounds" or "/share/sounds" with "${SOUND_INSTALL_DIR}"');
    $issues += &checkLine($line,$linecnt,
			  'DESTINATION[[:space:]]\${SOUND_INSTALL_DIR}/\$',
			  'replace "${SOUND_INSTALL_DIR}/${...}" with "${SOUND_INSTALL_DIR}/realname"');

    $issues += &checkLine($line,$linecnt,
                          'install_targets[[:space:]]*\(',
                          'replace "install_targets" with "install(TARGETS...)');

    $issues += &checkLine($line,$linecnt,
                          'INSTALL_TARGETS[[:space:]]*\(',
                          'replace "install_targets" with "install(TARGETS...)');
    $issues += &checkLine($line,$linecnt,
			  'install_files[[:space:]]*\(',
			  'replace "install_files" with "install(FILES...)');
    $issues += &checkLine($line,$linecnt,
			  'INSTALL_FILES[[:space:]]*\(',
			  'replace "install_files" with "install(FILES...)');
    $issues += &checkLine($line,$linecnt,
			  '\sFILES\sDESTINATION',
			  'missing list of files between FILES and DESTINATION');
    $issues += &checkLine($line,$linecnt,
			  '\sTARGETS\sDESTINATION',
			  'missing list of files between TARGETS and DESTINATION');

    $issues += &checkLine($line,$linecnt,
			  'DESTINATION\s\$\{INSTALL_TARGETS_DEFAULT_ARGS\}',
			  'remove DESTINATION keyword before ${INSTALL_TARGETS_DEFAULT_ARGS}');

    $issues += &checkLine($line,$linecnt,
			  'macro_bool_to_01[[:space:]]*\(.*[[:space:]][[:digit:]][[:space:]]*\)',
			  'do not use a digit as a variable');
    $issues += &checkLine($line,$linecnt,
			  'MACRO_BOOL_TO_01[[:space:]]*\(.*[[:space:]][[:digit:]][[:space:]]*\)',
			  'do not use a digit as a variable');
    $issues += &checkLine($line,$linecnt,
			  '-fexceptions',
			  'replace "-fexceptions" with "${KDE4_ENABLE_EXCEPTIONS}"');
    if ($in !~ m+/(phonon|okular|kopete|kdevelop|libkdegames)/+) {
      $issues +=
        &checkLine($line,$linecnt,
                   'set_target_properties.*PROPERTIES.*[[:space:]]VERSION[[:space:]][[:digit:]]',
                   'replace a hard-coded VERSION with "${GENERIC_LIB_VERSION}"');
      $issues +=
        &checkLine($line,$linecnt,
                   'set_target_properties.*PROPERTIES.*[[:space:]]SOVERSION[[:space:]][[:digit:]]',
                   'replace a hard-coded SOVERSION with "${GENERIC_LIB_SOVERSION}"');
    }

    #Qt variable
    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtDBus[\s/)]',
                 'replace "QtDBus" with "${QT_QTDBUS_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]Qt3Support[\s/)]',
                 'replace "Qt3Support" with "${QT_QT3SUPPORT_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtGui[\s/)]',
                 'replace "QtGui" with "${QT_QTGUI_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtNetwork[\s/)]',
                 'replace "QtNetwork" with "${QT_QTNETWORK_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtCore[\s/)]',
                 'replace "QtCore" with "${QT_QTCORE_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtSql[\s/)]',
                 'replace "QtSql" with "${QT_QTSQL_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtSvg[\s/)]',
                 'replace "QtSvg" with "${QT_QTSVG_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtSvg[\s/)]',
                 'replace "QtSvg" with "${QT_QTSVG_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtTest[\s/)]',
                 'replace "QtTest" with "${QT_QTTEST_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtXml[\s/)]',
                 'replace "QtXml" with "${QT_QTXML_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtScript[\s/)]',
                 'replace "QtScript" with "${QT_QTSCRIPT_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtAssistantClient[\s/)]',
                 'replace "QtAssistantClient" with "${QT_QTASSISTANTCLIENT_LIBRARY}"');
    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtHelp[\s/)]',
                 'replace "QtHelp" with "${QT_QTHELP_LIBRARY}"');
    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtWebKit[\s/)]',
                 'replace "QtWebKit" with "${QT_QTWEBKIT_LIBRARY}"');
    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtXmlPatterns[\s/)]',
                 'replace "QtXmlPatterns" with "${QT_QTXMLPATTERNS_LIBRARY}"');

    $issues +=
      &checkLine($line,$linecnt,
                 'target_link_libraries.*[[:space:]]QtMultimedia[\s/)]',
                 'replace "QtMultimedia" with "${QT_QTMULTIMEDIA_LIBRARY}"');


    # kdegames variables
    if (! $in_kdegames) {
      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kdegames[\s/)]',
                   'replace "kdegames" with "${KDEGAMES_LIBRARY}"');
    }

    # kdelibs variables
    if (! $in_kdelibs) {
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kdeui[\s/)]',
		   'replace "kdeui" with "${KDE4_KDEUI_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kio[\s/)]',
		   'replace "kio" with "${KDE4_KIO_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kdesu[\s/)]',
		   'replace "kdesu" with "${KDE4_KDESU_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]khtml[\s/)]',
		   'replace "khtml" with "${KDE4_KHTML_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kparts[\s/)]',
		   'replace "kparts" with "${KDE4_KPARTS_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kde3support[\s/)]',
		   'replace "kde3support" with "${KDE4_KDE3SUPPORT_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kutils[\s/)]',
		   'replace "kutils" with "${KDE4_KUTILS_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kdnssd[\s/)]',
		   'replace "kdnssd" with "${KDE4_KDNSSD_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]knewstuff2[\s/)]',
		   'replace "knewstuff2" with "${KDE4_KNEWSTUFF2_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]knewstuff3[\s/)]',
                   'replace "knewstuff3" with "${KDE4_KNEWSTUFF3_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]knotifyconfig[\s/)]',
		   'replace "knotifyconfig" with "${KDE4_KNOTIFYCONFIG_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]threadweaver[\s/)]',
		   'replace "threadweaver" with "${KDE4_THREADWEAVER_LIBRARIES}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]krosscore[\s/)]',
		   'replace "krosscore" with "${KDE4_KROSSCORE_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]krossui[\s/)]',
		   'replace "krossui" with "${KDE4_KROSSUI_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]phonon[\s/)]',
		   'replace "phonon" with "${PHONON_LIBRARY}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kaudiodevicelist[\s/)]',
		   'replace "kaudiodevicelist" with "${KDE4_KAUDIODEVICELIST_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]solidifaces[\s/)]',
		   'replace "solidifaces" with "${KDE4_SOLIDIFACES_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]solid[\s/)]',
		   'replace "solid" with "${KDE4_SOLID_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]ktexteditor[\s/)]',
		   'replace "ktexteditor" with "${KDE4_KTEXTEDITOR_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kfile[\s/)]',
		   'replace "kfile" with "${KDE4_KFILE_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]knepomuk[\s/)]',
		   'replace "knepomuk" with "${KDE4_KNEPOMUK_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kmetadata[\s/)]',
		   'replace "kmetadata" with "${KDE4_KMETADATA_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kjs[\s/)]',
		   'replace "kjs" with "${KDE4_KJS_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kjsapi[\s/)]',
		   'replace "kjsapi" with "${KDE4_KJSAPI_LIBS}"');

      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kformula[\s/)]',
		   'replace "kformula" with "${KDE4_KFORMULA_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]plasma[\s/)]',
                   'replace "plasma" with "${KDE4_PLASMA_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kunitconversion[\s/)]',
                   'replace "kunitconversion" with "${KDE4_KUNITCONVERSION_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kdewebkit[\s/)]',
                   'replace "kdewebkit" with "${KDE4_KDEWEBKIT_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kprintutils[\s/)]',
                   'replace "kprintutils" with "${KDE4_KPRINTUTILS_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kcmutils[\s/)]',
                   'replace "kcmutils" with "${KDE4_KCMUTILS_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kemoticons[\s/)]',
                   'replace "kemoticons" with "${KDE4_KEMOTICONS_LIBS}"');

      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kidletime[\s/)]',
                   'replace "kidletime" with "${KDE4_KIDLETIME_LIBS}"');

    }

    # kdepimlibs variables
    if (! $in_kdelibs && ! $in_kdepimlibs) {
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]akonadi-kde[\s/)]',
		   'replace "akonadi-kde" with "${KDEPIMLIBS_AKONADI_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]akonadi-kmime[\s/)]',
		   'replace "akonadi-kmime" with "${KDEPIMLIBS_AKONADI_KMIME_LIBS}"');
      $issues += 
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]gpgmepp[\s/)]',
		   'replace "gpgmepp" with "${KDEPIMLIBS_GPGMEPP_LIBS}"');
      $issues += 
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kabc[\s/)]',
		   'replace "kabc" with "${KDEPIMLIBS_KABC_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kblog[\s/)]',
		   'replace "kblog" with "${KDEPIMLIBS_KBLOG_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kcal[\s/)]',
		   'replace "kcal" with "${KDEPIMLIBS_KCAL_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kholidays[\s/)]',
		   'replace "kholidays" with "${KDEPIMLIBS_KHOLIDAYS_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kimap[\s/)]',
		   'replace "kimap" with "${KDEPIMLIBS_KIMAP_LIBS}"');
      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]kldap[\s/)]',
                   'replace "kldap" with "${KDEPIMLIBS_KLDAP_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kmime[\s/)]',
		   'replace "kmime" with "${KDEPIMLIBS_KMIME_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kontactinterface[\s/)]',
		   'replace "kontactinterface" with "${KDEPIMLIBS_KONTACTINTERFACE_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kpimidentities[\s/)]',
		   'replace "kpimidentities" with "${KDEPIMLIBS_KPIMIDENTITIES_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kpimtextedit[\s/)]',
		   'replace "kpimtextedit" with "${KDEPIMLIBS_KPIMTEXTEDIT_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kpimutils[\s/)]',
		   'replace "kpimutils" with "${KDEPIMLIBS_KPIMUTILS_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kresources[\s/)]',
		   'replace "kresources" with "${KDEPIMLIBS_KRESOURCES_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]ktnef[\s/)]',
		   'replace "ktnef" with "${KDEPIMLIBS_KTNEF_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]kxmlrpcclient[\s/)]',
		   'replace "kxmlrpcclient" with "${KDEPIMLIBS_KXMLRPCCLIENT_LIBS}"');
      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]mailtransport[\s/)]',
                   'replace "mailtransport" with "${KDEPIMLIBS_MAILTRANSPORT_LIBS}"');
      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]microblog[\s/)]',
                   'replace "microblog" with "${KDEPIMLIBS_MICROBLOG_LIBS}"');
      $issues +=
	&checkLine($line,$linecnt,
		   'target_link_libraries.*[[:space:]]qgpgme[\s/)]',
		   'replace "qgpgme" with "${KDEPIMLIBS_QGPGME_LIBS}"');
      $issues +=
        &checkLine($line,$linecnt,
                   'target_link_libraries.*[[:space:]]syndication[\s/)]',
                   'replace "syndication" with "${KDEPIMLIBS_SYNDICATION_LIBS}"');
    }

    if ($line !~ m+(Qt4|IOKit|KdeSubversion|KDE4|KDE4Internal|KDEWIN32|KdepimLibs|kdevplatform|Carbon|Gpgme)+ && $in !~ m+/(examples|qtonly)/+) {
      $issues +=
       &checkLine($line,$linecnt,
                   '^\s*[Ff][Ii][Nn][Dd]_[Pp][Aa][Cc][Kk][Aa][Gg][Ee]\s*\(\s*[A-Za-z0-9_]*\sREQUIRED\s*\)',
                  'Do not use the REQUIRED keyword with find_package()');
    }

    my($subdir);
    if ($line =~ m+macro_optional_add_subdirectory\s*\(\s*(\S*)\s*\)+) {
      $subdir = $1;
      if (!&canBeOptional($subdir) || ($top_of_module && ($in_kdelibs || $in_kdepimlibs))) {
	if (!&mustBeOptional($subdir)) {
	  $issues++;
	  &printIssue($line,$linecnt,"Replace macro_optional_add_subdirectory($subdir) with add_subdirectory($subdir)");
	}
      }
    }
    if ($line =~ m+^\s*add_subdirectory\s*\(\s*(\S*)\s*\)+) {
      $subdir = $1;
      if (&mustBeOptional($subdir)) {
	$issues++;
	&printIssue($line,$linecnt,"Replace add_subdirectory($subdir) with macro_optional_add_subdirectory($subdir)");
      }
    }
    $prevline = $line;
  }

  #look for "missing" stuff
  my($in_exec)=0;
  my($has_project)=0;
  my($has_display_log)=0;
  foreach $line (@lines) {
    chomp($line);
    $line =~ s/#.*$//; #remove comments
    next if ($line =~ m/^[[:space:]]$/); #skip blank lines
    $in_exec = 1
      if ($line =~ m/add_(|kdeinit_)executable[[:space:]]*\(/i);
    if ($line =~ m/[Pp][Rr][Oo][Jj][Ee][Cc][Tt]/) {
      $has_project=1;
    }
    if ($line =~ m/macro_display_feature_log/i) {
      $has_display_log=1;
    }
  }
  if (! $has_project && $top_of_project && $in_exec) {
    $issues++;
    &printIssue($line,$linecnt,"Missing a PROJECT() command");
  }
  if ($top_of_module && $has_display_log == 0) {
    $issues++;
    &printIssue($line,$linecnt,"Missing macro_display_feature_log() command");
  }
  #if (!$top_of_module && $has_display_log == 1 && $in !~ m+/qtonly/+) {
  #  $issues++;
  #  &printIssue($line,$linecnt,"Do not put macro_display_feature_log() in a subdir CMakeLists.txt");
  #}
  if ($nop != $ncp) {
    $issues++;
    &printIssue($line,$linecnt,"Mismatched parens");
  }
  if ($nob != $ncb) {
    $issues++;
    &printIssue($line,$linecnt,"Mismatched braces");
  }

  #missing macro_log_feature()
  foreach $pack ( keys %optpacks ) {
    if ($optpacks{$pack}{'log'} == 0) {
      $issues++;
      &printIssue("","","Missing macro_log_feature($pack)");
    }
  }

  close(IN);
  return $issues;
}

sub checkLine {
  my($line,$cnt,$regex,$explain) = @_;
  if ($line =~ m/$regex/i) {
    &printIssue($line,$cnt,$explain);
    return 1;
  }
  return 0;
}

sub printIssue {
  my($line,$cnt,$explain) = @_;
  if ($line) {
    print "\tline#$cnt: $explain\n";
    print "\t=>$line\n" if ($verbose);
  } else {
    print "\t$explain\n";
  }
}

sub canBeOptional {
  my($guy) = @_;

  my($ret) = 1;
  $ret = 0
    if ($guy =~ m/^lib/ ||
	$guy =~ m/lib$/ ||
        $guy =~ m/^cmake$/
       );
  return $ret;
}

sub mustBeOptional {
  my($guy) = @_;

  my($ret) = 0;
  $ret = 1
    if ($guy =~ m/^doc$/);
  return $ret;
}

#==============================================================================
# Help function: print help message and exit.
sub Help {
  &Version();
  print "Check KDE CMakeLists.txt files for common errors.\n\n";
  print "Usage: $Prog [OPTIONS] FILES\n";
  print "  --help             display help message and exit\n";
  print "  --version          display version information and exit\n";
  print "\n";
  exit 0 if $help;
}

# Version function: print the version number and exit.
sub Version {
  print "$Prog, version $Version\n";
  exit 0 if $version;
}

__END__
