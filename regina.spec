Name: regina
Summary: 3-manifold topology software with normal surface support
Version: 4.1.2
Release: 1.%{_vendor}
License: GPL
Group: Applications/Utilities
Source: http://prdownloads.sourceforge.net/regina/%{name}-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

BuildRequires: boost-devel
# BuildRequires: cppunit
BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: gmp-devel
BuildRequires: junk
BuildRequires: kdelibs-devel
BuildRequires: libselinux-devel
BuildRequires: libstdc++-devel
BuildRequires: libxml2-devel
BuildRequires: popt
BuildRequires: python-devel
BuildRequires: qt-devel >= 3.2
BuildRequires: zlib-devel

Prereq: /sbin/ldconfig

%description
Regina is a suite of mathematical software for 3-manifold topologists.
It focuses upon the study of 3-manifold triangulations and includes
support for normal surfaces and angle structures.

Highlights of Regina include triangulation analysis and simplification,
census creation and normal surface enumeration.  It offers embedded
Python scripting giving full access to the calculation engine.

%prep
rm -rf $RPM_BUILD_ROOT
%setup -n %{name}-%{version}
CFLAGS="" CXXFLAGS="" ./configure --disable-debug

%build
make

%install
make install-strip DESTDIR=$RPM_BUILD_ROOT

cd 
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > /%{name}-master.list
find . -type f -o -type l | sed 's|^\.||' >> $RPM_BUILD_DIR/%{name}-master.list

%clean
rm -rf $RPM_BUILD_DIR/%{name}-%{version}
rm -rf $RPM_BUILD_DIR/-master.list

%files -f $RPM_BUILD_DIR/%{name}-master.list
