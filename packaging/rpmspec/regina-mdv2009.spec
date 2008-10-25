# Known to work for:
# - Mandriva 2009.0 (i586, x86_64)

Name: regina-normal
Summary: 3-manifold topology software with normal surface support
Version: 4.5.1
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Sciences/Mathematics
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

Requires: kdebase-progs
Requires: kdegraphics-kpdf
Requires: python
Conflicts: regina

BuildConflicts: regina-normal < %{version}
BuildConflicts: regina-normal > %{version}
BuildRequires: boost-devel
BuildRequires: cppunit-devel
BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: gmp-devel
BuildRequires: kdelibs-devel
BuildRequires: libstdc++-devel
BuildRequires: libxml2-devel
BuildRequires: qt3-devel >= 3.2
BuildRequires: popt-devel
BuildRequires: python-devel
BuildRequires: zlib1-devel

Prereq: /sbin/ldconfig

%description
Regina is a suite of mathematical software for 3-manifold topologists.
It focuses upon the study of 3-manifold triangulations and includes
support for normal surfaces and angle structures.

Highlights of Regina include triangulation analysis and simplification,
census creation and normal surface enumeration.  It offers embedded
Python scripting giving full access to the calculation engine.

%prep
%setup -n regina-%{version}

%build
unset QTINC || : ; unset QTLIB || : ; export QTDIR=/usr/lib/qt3/
FLAGS="$RPM_OPT_FLAGS -DNDEBUG -DNO_DEBUG"
export CFLAGS="$FLAGS"
export CXXFLAGS="$FLAGS"
./configure \
%if "%{_lib}" != "lib"
  --enable-libsuffix="%(A=%{_lib}; echo ${A/lib/})" \
%endif
  --disable-mpi --disable-debug --mandir=%{_mandir} --includedir=%{_includedir}

# Stop for a sanity check to see if the right bits are going to be built.
grep '^REGINA_BUILD_DOCSENGINE=.engine.$' config.log > /dev/null
grep '^REGINA_BUILD_ENGINE=.engine.$' config.log > /dev/null
grep '^REGINA_BUILD_KDEUI=.kdeui.$' config.log > /dev/null
grep '^REGINA_BUILD_MPI=..$' config.log > /dev/null
grep '^REGINA_BUILD_PYTHON=.python.$' config.log > /dev/null
grep '^REGINA_BUILD_TESTSUITE=.testsuite.$' config.log > /dev/null
grep '^REGINA_BUILD_UTILS=.utils.$' config.log > /dev/null

# On with the show.
make
make check

%install
rm -rf $RPM_BUILD_ROOT
make install-strip DESTDIR=$RPM_BUILD_ROOT

# Delete unnecessary static libraries.
rm -f $RPM_BUILD_ROOT%{_kde3_libdir}/libregina-kdecommon.a
rm -f $RPM_BUILD_ROOT%{_kde3_libdir}/regina/python/regina.a
rm -f $RPM_BUILD_ROOT%{_kde3_libdir}/kde3/libreginapart.a

# Delete library files that can cause unnecessary dependencies.
rm -f $RPM_BUILD_ROOT%{_kde3_libdir}/libregina-kdecommon.la
rm -f $RPM_BUILD_ROOT%{_kde3_libdir}/libregina-kdecommon.so

# Move regina-engine-config from /opt/kde3/bin into /usr/bin.
install -m755 -d $RPM_BUILD_ROOT%{_bindir}
mv $RPM_BUILD_ROOT%{_kde3_bindir}/regina-engine-config $RPM_BUILD_ROOT%{_bindir}

# Multiarch support.
%multiarch_binaries $RPM_BUILD_ROOT%{_bindir}/regina-engine-config
%multiarch_includes $RPM_BUILD_ROOT%{_includedir}/regina/regina-config.h

%post
/sbin/ldconfig
%{update_menus}
%{update_desktop_database}
%update_kde3_icon_cache crystalsvg
%update_kde3_icon_cache hicolor

%postun
/sbin/ldconfig
%{clean_menus}
%{update_desktop_database}
%clean_kde3_icon_cache crystalsvg
%clean_kde3_icon_cache hicolor

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc AUTHORS.txt
%doc CHANGES.txt
%doc HIGHLIGHTS.txt
%doc LICENSE.txt
%docdir %{_docdir}/HTML/en/regina
%docdir %{_datadir}/regina/engine-docs
# Make sure we don't catch the multiarch_bin directory.
%{_bindir}/reg*
%{_datadir}/man/*/*
%{_includedir}/regina
%{_kde3_bindir}/*
%{_kde3_libdir}/libregina*
# Make sure we don't ship unwanted static libs by accident.
%{_kde3_libdir}/kde3/libreginapart.la
%{_kde3_libdir}/kde3/libreginapart.so
%{_kde3_libdir}/regina/python/regina.la
%{_kde3_libdir}/regina/python/regina.so
%{_kde3_docdir}/HTML/en/regina
%{_kde3_datadir}/applications/kde/*
%{_kde3_datadir}/apps/regina
%{_kde3_datadir}/apps/reginapart
%{_kde3_datadir}/icons/*/*/*/*
%{_kde3_datadir}/mimelnk/*/*
%{_kde3_datadir}/regina
%{_kde3_datadir}/services/*

# Multiarch support:
%multiarch %{multiarch_bindir}/regina-engine-config
%multiarch %{multiarch_includedir}/regina/regina-config.h

%changelog
* Tue Oct 28 2008 Ben Burton <bab@debian.org> 4.5.1
- New upstream release.
- Now requires kdegraphics-kpdf, which provides an embedded viewer for
  Regina's new PDF packets.

* Sat May 17 2008 Ben Burton <bab@debian.org> 4.5
- New upstream release.

* Sun Nov 25 2007 Ben Burton <bab@debian.org> 4.4
- New upstream release.
- Removed MPI-enabled utilities from packages, since this causes hassles
  for ordinary desktop users who need to hunt down MPICH dependencies.

* Fri May 5 2006 Ben Burton <bab@debian.org> 4.3.1
- New upstream release.

* Mon Mar 27 2006 Ben Burton <bab@debian.org> 4.3
- New upstream release.

* Sun Sep 18 2005 Ben Burton <bab@debian.org> 4.2.1
- New upstream release.

* Thu Jul 7 2005 Ben Burton <bab@debian.org> 4.2
- New upstream release.
- Reenabled Python scripting for Mandrake >= 10.1.
- Note that regina-normal now includes MPI support.  These packages are
  built against the MPICH implementation of MPI.

* Sun Jul 25 2004 Ben Burton <bab@debian.org> 4.1.3
- New upstream release.

* Fri Jun 25 2004 Ben Burton <bab@debian.org> 4.1.2
- Initial packaging using Mandrake 10.0 Official.
- Python scripting is initially disabled because of bugs in Mandrake 10.0's
  boost.python packaging (see Mandrake bug #9648).
