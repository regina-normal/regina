# Known to work for:
# - SuSE 11.4 (i586, x86_64)
# - SuSE 11.3 (i586, x86_64)

Name: regina-normal
Summary: Software for 3-manifold topology and normal surfaces
Version: 4.90
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Applications/Engineering
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

%kde4_runtime_requires
Requires: graphviz
Requires: okular
Requires: python
Conflicts: regina

BuildRequires: boost-devel
BuildRequires: cmake
BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: gmp-devel
BuildRequires: libcppunit-devel
BuildRequires: libkde4-devel
BuildRequires: libqt4-devel
BuildRequires: libstdc++-devel
BuildRequires: libxml2-devel
BuildRequires: popt-devel
BuildRequires: python-devel
BuildRequires: shared-mime-info
BuildRequires: zlib-devel

Prereq: /sbin/ldconfig

%description
Regina is a suite of mathematical software for 3-manifold topologists.
It focuses on the study of 3-manifold triangulations and normal surfaces.

Other highlights of Regina include angle structures, census enumeration,
combinatorial recognition of triangulations, and high-level tasks such
as 3-sphere recognition and connected sum decomposition.  Regina comes
with a full graphical user interface, and also offers Python bindings
and a low-level C++ programming interface.

%prep
%setup -n regina-%{version}

%build
%cmake_kde4 -d build -- -DPACKAGING_MODE=1 -DPACKAGING_NO_MPI=1
%make_jobs
LD_LIBRARY_PATH=`pwd`/engine:"$LD_LIBRARY_PATH" make %{?_smp_mflags} VERBOSE=1 test ARGS=-V

%install
pushd build
%makeinstall
popd

%kde_post_install

%post
/sbin/ldconfig

%if 0%{?suse_version} <= 1130
  %{_bindir}/update-desktop-database --quiet "%{_datadir}/applications" || true
%else
  %desktop_database_post
%endif

# Hand-roll our own update-mime-database so we can pipe output to /dev/null.
%{_bindir}/update-mime-database "%{_datadir}/mime" &> /dev/null || true

%if 0%{?suse_version} <= 1130
  if test -x %{_bindir}/gtk-update-icon-cache; then
    %{_bindir}/gtk-update-icon-cache --quiet --force "%{_datadir}/icons/hicolor" || true
  fi
%else
  %icon_theme_cache_post
%endif

%postun
/sbin/ldconfig

%if 0%{?suse_version} <= 1130
  %{_bindir}/update-desktop-database --quiet "%{_datadir}/applications" || true
%else
  %desktop_database_postun
%endif

# Hand-roll our own update-mime-database so we can pipe output to /dev/null.
%{_bindir}/update-mime-database "%{_datadir}/mime" &> /dev/null || true

%if 0%{?suse_version} <= 1130
  if test -x %{_bindir}/gtk-update-icon-cache; then
    %{_bindir}/gtk-update-icon-cache --quiet --force "%{_datadir}/icons/hicolor" || true
  fi
%else
  %icon_theme_cache_postun
%endif

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc CHANGES.txt
%doc HIGHLIGHTS.txt
%doc LICENSE.txt
%docdir %{_datadir}/regina/engine-docs
%docdir %{_kde4_docdir}/HTML/en/regina
%docdir %{_kde4_docdir}/HTML/en/regina-xml
%{_bindir}/*
%{_datadir}/applications/kde4/regina.desktop
%{_datadir}/mime/packages/regina.xml
%{_datadir}/regina/
%{_includedir}/regina/
%{_libdir}/libregina-engine.so
%{_libdir}/libregina-engine.so.%{version}
%{_libdir}/regina/python/regina.so
%{_mandir}/*/*
%{_kde4_appsdir}/regina/
%{_kde4_docdir}/HTML/en/regina/
%{_kde4_docdir}/HTML/en/regina-xml/
%{_kde4_iconsdir}/*/*/*

%changelog
* Mon Sep 12 2011 Ben Burton <bab@debian.org> 4.90
- New upstream release.
- Ported from KDE3 to KDE4, and from autotools to cmake.

* Sat May 16 2009 Ben Burton <bab@debian.org> 4.6
- New upstream release.

* Tue Oct 28 2008 Ben Burton <bab@debian.org> 4.5.1
- New upstream release.
- Now requires kdegraphics3-pdf, which provides an embedded viewer for
  Regina's new PDF packets.

* Sun Jun 29 2008 Ben Burton <bab@debian.org> 4.5 (SuSE 11.0)
- Packaging the 4.5 release (May 2008) for SuSE 11.0.
- Note that regina-normal needs to be built against boost 1.34.1-42.2 (or
  later) from the updates repository, since the boost packages originally
  shipped with SuSE 11.0 are broken.  See
  https://bugzilla.novell.com/show_bug.cgi?id=401964 for details.

* Sat May 17 2008 Ben Burton <bab@debian.org> 4.5
- New upstream release.

* Sun Nov 25 2007 Ben Burton <bab@debian.org> 4.4
- New upstream release.
- Removed MPI-enabled utilities from packages, since this causes hassles
  for ordinary desktop users who need to hunt down MPICH dependencies.

* Sun Feb 4 2007 Ben Burton <bab@debian.org> 4.3.1
- Packaging the 4.3.1 release (May 2006) for SuSE 10.2.
- Reenabled Python scripting for SuSE 10.1 and 10.2, since SuSE has
  fixed their boost packages once more.

* Mon Mar 27 2006 Ben Burton <bab@debian.org> 4.3
- New upstream release.
- Python scripting is again disabled because SuSE 10.0's boost packages
  are broken (https://bugzilla.novell.com/show_bug.cgi?id=137558).
  SuSE claims that this will be fixed in SuSE 10.1.

* Sun Sep 18 2005 Ben Burton <bab@debian.org> 4.2.1
- New upstream release.

* Thu Jul 7 2005 Ben Burton <bab@debian.org> 4.2
- New upstream release.
- Reenabled Python scripting for SuSE >= 9.2.
- Note that regina-normal now includes MPI support.  These packages are
  built against the MPICH implementation of MPI with the ch-p4 device.

* Sun Jul 25 2004 Ben Burton <bab@debian.org> 4.1.3
- New upstream release.
- Built against an updated popt from YaST online updates.  The earlier
  popt packages from SuSE used an incorrect soname (libpopt.so.1).  The
  updated popt packages fix this (they now use libpopt.so.0).  Because
  this bugfix from SuSE changes the soname, regina-normal will require
  the updated popt package, and will no longer be able to use the
  original popt from SuSE 9.1.

* Sun Jun 27 2004 Ben Burton <bab@debian.org> 4.1.2
- Initial packaging using SuSE 9.1.
- Python scripting is initially disabled because of bugs in SuSE 9.1's
  C++ compiler (SuSE applies patches to g++ that inadvertently break
  Boost.Python).  For a fuller explanation, see:
  http://mail.python.org/pipermail/c++-sig/2004-June/007561.html
