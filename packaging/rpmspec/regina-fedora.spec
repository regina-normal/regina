# Known to work for:
# - Fedora 15 (i386, x86_64)

Name: regina-normal
Summary: 3-manifold topology software with normal surface support
Version: 4.90
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Applications/Engineering
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

Requires: graphviz
Requires: kdelibs
Requires: kdebase-runtime
Requires: python
Conflicts: regina

BuildRequires: boost-devel
BuildRequires: cmake
BuildRequires: cppunit-devel
BuildRequires: desktop-file-utils
BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: gmp-devel
BuildRequires: kdelibs-devel
BuildRequires: libstdc++-devel
BuildRequires: libxml2-devel
BuildRequires: popt-devel
BuildRequires: python-devel
BuildRequires: zlib-devel

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
mkdir -p %{_target_platform}
pushd %{_target_platform}
%{cmake_kde4} .. -DPACKAGING_MODE=1 -DPACKAGING_NO_MPI=1
popd

make %{?_smp_mflags} -C %{_target_platform}
make %{?_smp_mflags} -C %{_target_platform} test ARGS=-V

%install
rm -rf $RPM_BUILD_ROOT
make install/fast DESTDIR=$RPM_BUILD_ROOT -C %{_target_platform}

desktop-file-install \
  --dir $RPM_BUILD_ROOT%{_datadir}/applications/kde4 \
  $RPM_BUILD_ROOT%{_datadir}/applications/kde4/*

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc CHANGES.txt
%doc HIGHLIGHTS.txt
%doc LICENSE.txt
%docdir %{_kde4_docdir}/HTML/en/regina
%docdir %{_kde4_docdir}/HTML/en/regina-xml
%docdir %{_datadir}/regina/engine-docs
%{_bindir}/*
%{_datadir}/regina/
%{_includedir}/regina/
%{_libdir}/libregina-engine.so
%{_libdir}/libregina-engine.so.%{version}
%{_libdir}/regina/python/regina.so
%{_kde4_docdir}/HTML/en/regina/
%{_kde4_docdir}/HTML/en/regina-xml/
%{_kde4_appsdir}/regina/
%{_kde4_datadir}/applications/kde4/regina.desktop
%{_mandir}/*/*
%{_kde4_datadir}/mime/packages/regina.xml
%{_kde4_iconsdir}/*/*/*

%changelog
* Sat Sep 10 2011 Ben Burton <bab@debian.org> 4.90
- New upstream release.
- Ported from KDE3 to KDE4, and from autotools to cmake.

* Wed Jul 15 2009 Ben Burton <bab@debian.org> 4.6
- Built for Fedora 11, which was released last month.

* Sat May 16 2009 Ben Burton <bab@debian.org> 4.6
- New upstream release.

* Wed Dec 10 2008 Ben Burton <bab@debian.org> 4.5.1
- Built for Fedora 10, which was released last month.

* Tue Oct 28 2008 Ben Burton <bab@debian.org> 4.5.1
- New upstream release.
- It will help to have KPDF installed, which provides an embedded viewer
  for Regina's new PDF packets.  However, the regina-normal packages for
  Fedora do not list KPDF as a dependency.  This is because:
  + Fedora <= 8 only ships KPDF as part of the monolithic kdegraphics
    package, which is very large.
  + Fedora >= 9 does not ship KPDF at all, but instead focuses on its KDE 4
    successor.
  Regina can find other ways of viewing PDF packets; see Regina's PDF settings
  for details.

* Sat May 17 2008 Ben Burton <bab@debian.org> 4.5
- New upstream release.

* Sun Nov 25 2007 Ben Burton <bab@debian.org> 4.4
- New upstream release.
- Removed MPI-enabled utilities from packages, since this causes hassles
  for ordinary desktop users who need to hunt down LAM dependencies.

* Fri May 5 2006 Ben Burton <bab@debian.org> 4.3.1
- New upstream release.

* Mon Mar 27 2006 Ben Burton <bab@debian.org> 4.3
- New upstream release.

* Sun Sep 18 2005 Ben Burton <bab@debian.org> 4.2.1
- New upstream release.

* Thu Jul 7 2005 Ben Burton <bab@debian.org> 4.2
- New upstream release.
- Note that regina-normal now includes MPI support.  These packages are
  built against the LAM implementation of MPI.

* Sun Jul 25 2004 Ben Burton <bab@debian.org> 4.1.3
- New upstream release.

* Fri Jun 11 2004 Ben Burton <bab@debian.org> 4.1.2
- Initial packaging using Fedora Core 2.
