# Known to work for:
# - Mandrake 10.0

Name: regina-normal
Summary: 3-manifold topology software with normal surface support
Version: 4.1.2
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Applications/Engineering
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

Requires: kdebase
Requires: python

BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: libboost1-devel
BuildRequires: libcppunit1.8-devel
BuildRequires: libgmp3-devel
BuildRequires: libkdecore4-devel
BuildRequires: libpython2.3-devel
BuildRequires: libqt3-devel >= 3.2
BuildRequires: libselinux1-devel
BuildRequires: libstdc++5-devel
BuildRequires: libxml2-devel
BuildRequires: popt-devel
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
unset QTDIR || : ; . /etc/profile.d/qt.sh
FLAGS="$RPM_OPT_FLAGS -DNDEBUG -DNO_DEBUG"
export CFLAGS="$FLAGS"
export CXXFLAGS="$FLAGS"
./configure PKGNAME=regina-normal --disable-debug --mandir=%{_mandir}
make
make check

%install
rm -rf $RPM_BUILD_ROOT
make install-strip DESTDIR=$RPM_BUILD_ROOT

# Delete some huge and unnecessary static libraries.
rm -f $RPM_BUILD_ROOT%{_libdir}/regina-normal/python/regina.a
rm -f $RPM_BUILD_ROOT%{_libdir}/kde3/libreginapart.a

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
%docdir %{_docdir}/HTML/en/regina
%docdir %{_datadir}/regina-normal/engine-docs
%{_bindir}/*
%{_includedir}/regina-normal
%{_libdir}/libregina*
# Make sure we don't ship unwanted static libs by accident.
%{_libdir}/kde3/libreginapart.la
%{_libdir}/kde3/libreginapart.so
%{_libdir}/regina-normal/python/regina.la
%{_libdir}/regina-normal/python/regina.so
%{_docdir}/HTML/en/regina
# %{_datadir}/applications/*
%{_datadir}/applnk/*/*
%{_datadir}/apps/regina
%{_datadir}/apps/reginapart
%{_datadir}/icons/*/*/*/*
%{_datadir}/man/*/*
%{_datadir}/mimelnk/*/*
%{_datadir}/regina-normal
%{_datadir}/services/*

%changelog
* Thu Jun 24 2004 Ben Burton <bab@debian.org> 4.1.2
- Initial packaging using Mandrake 10.0 Official.
