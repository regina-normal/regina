# Known to work for:
# - Mandriva 2011 (i586, x86_64)
# - Mandriva 2010.2 (i586, x86_64)

Name: regina-normal
Summary: Software for 3-manifold topology and normal surfaces
Version: 4.90
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Sciences/Mathematics
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

Requires: graphviz
Requires: kdebase4-runtime
Requires: kdelibs4-core
Requires: okular
Requires: python
Conflicts: regina

BuildRequires: boost-devel
BuildRequires: cmake
BuildRequires: cppunit-devel
BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: gmp-devel
BuildRequires: kdelibs4-devel
BuildRequires: libstdc++-devel
BuildRequires: libxml2-devel
BuildRequires: qt4-devel
BuildRequires: popt-devel
BuildRequires: python-devel
BuildRequires: shared-mime-info
BuildRequires: zlib1-devel

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
%cmake_kde4 -DPACKAGING_MODE=1 -DPACKAGING_NO_MPI=1
%make
LD_LIBRARY_PATH=`pwd`/engine:"$LD_LIBRARY_PATH" %make test ARGS=-V

%install
rm -rf %{buildroot}
%makeinstall_std -C build

%post
/sbin/ldconfig
%update_menus
%update_desktop_database
%update_mime_database
%update_icon_cache hicolor

%postun
/sbin/ldconfig
%clean_menus
%clean_desktop_database
%clean_mime_database
%update_icon_cache hicolor

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc CHANGES.txt
%doc HIGHLIGHTS.txt
%doc LICENSE.txt
%docdir %{_datadir}/regina/engine-docs
%docdir %{_kde4_docdir}/HTML/en/regina
%docdir %{_kde4_docdir}/HTML/en/regina-xml
%{_bindir}/*
%{_datadir}/regina/
%{_includedir}/regina/
%{_libdir}/libregina-engine.so
%{_libdir}/libregina-engine.so.%{version}
%{_libdir}/regina/python/regina.so
%{_mandir}/*/*
%{_kde_appsdir}/regina/
%{_kde_datadir}/applications/kde4/regina.desktop
%{_kde_datadir}/mime/packages/regina.xml
%{_kde_docdir}/HTML/en/regina/
%{_kde_docdir}/HTML/en/regina-xml/
%{_kde_iconsdir}/*/*/*

%changelog
* Mon Sep 12 2011 Ben Burton <bab@debian.org> 4.90
- New upstream release.
- Ported from KDE3 to KDE4, and from autotools to cmake.

* Sat May 16 2009 Ben Burton <bab@debian.org> 4.6
- New upstream release.

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
