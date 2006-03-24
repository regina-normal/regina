# Known to work for:
# - SuSE 9.2
# - SuSE 9.3

Name: regina-normal
Summary: 3-manifold topology software with normal surface support
Version: 4.3
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Applications/Engineering
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Patch: regina-mpich_ch-p4.patch
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

Requires: kdelibs3
Requires: kdebase3
Requires: python
Conflicts: regina

BuildRequires: boost
BuildRequires: boost-devel
BuildRequires: cppunit
BuildRequires: cppunit-devel
BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: gmp
BuildRequires: kdelibs3-devel >= 3.2
BuildRequires: libjpeg-devel
BuildRequires: libstdc++-devel
BuildRequires: libxml2-devel
BuildRequires: mpich
BuildRequires: mpich-devel
BuildRequires: popt
BuildRequires: popt-devel
BuildRequires: python-devel
BuildRequires: qt3-devel >= 3.2
BuildRequires: zlib-devel

Prereq: /sbin/ldconfig

%define _prefix /opt/kde3
%define _kdedocdir /opt/kde3/share/doc
%define _includedir /usr/include
%define _mandir /usr/share/man

%description
Regina is a suite of mathematical software for 3-manifold topologists.
It focuses upon the study of 3-manifold triangulations and includes
support for normal surfaces and angle structures.

Highlights of Regina include triangulation analysis and simplification,
census creation and normal surface enumeration.  It offers embedded
Python scripting giving full access to the calculation engine.

%prep
%setup -n regina-%{version}
%patch

%build
FLAGS="$RPM_OPT_FLAGS -DNDEBUG -DNO_DEBUG"
export CFLAGS="$FLAGS"
export CXXFLAGS="$FLAGS"
./configure --disable-debug --includedir=%{_includedir} --mandir=%{_mandir} MPICXX=/opt/mpich/ch-p4/bin/mpicxx
make
make check

%install
rm -rf $RPM_BUILD_ROOT
make install-strip DESTDIR=$RPM_BUILD_ROOT

# Make a symlink to the Regina data directory where
# people might actually be able to find it.
ln -s /opt/kde3/share/regina $RPM_BUILD_ROOT/usr/share/regina

# Delete unnecessary static libraries.
rm -f $RPM_BUILD_ROOT%{_libdir}/libregina-kdecommon.a
rm -f $RPM_BUILD_ROOT%{_libdir}/regina/python/regina.a
rm -f $RPM_BUILD_ROOT%{_libdir}/kde3/libreginapart.a

# Delete library files that can cause unnecessary dependencies.
rm -f $RPM_BUILD_ROOT%{_libdir}/libregina-kdecommon.la
rm -f $RPM_BUILD_ROOT%{_libdir}/libregina-kdecommon.so

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc AUTHORS.txt
%doc CHANGES.txt
%doc HIGHLIGHTS.txt
%doc LICENSE.txt
%doc README.MPI
%docdir %{_kdedocdir}/HTML/en/regina
%docdir %{_datadir}/regina/engine-docs
%{_bindir}/*
%{_includedir}/regina
%{_libdir}/libregina*
# Make sure we don't ship unwanted static libs by accident.
%{_libdir}/kde3/libreginapart.la
%{_libdir}/kde3/libreginapart.so
%{_libdir}/regina/python/regina.la
%{_libdir}/regina/python/regina.so
%{_kdedocdir}/HTML/en/regina
%{_datadir}/applications/kde/*
%{_datadir}/apps/regina
%{_datadir}/apps/reginapart
%{_datadir}/icons/*/*/*/*
%{_mandir}/*/*
%{_datadir}/mimelnk/*/*
%{_datadir}/regina
%{_datadir}/services/*
# Don't forget the symlink.
/usr/share/regina

%changelog
* Mon Mar 27 2006 Ben Burton <bab@debian.org> 4.3
- New upstream release.

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
