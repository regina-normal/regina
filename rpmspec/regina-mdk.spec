# Known to work for:
# - Mandrake 10.0

Name: regina-normal
Summary: 3-manifold topology software with normal surface support
Version: 4.1.3
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Sciences/Mathematics
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

Requires: kdebase-progs
# Requires: python
Conflicts: regina

BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
# BuildRequires: libboost1-devel
BuildRequires: libcppunit1.8-devel
BuildRequires: libgmp3-devel
BuildRequires: libkdecore4-devel
# BuildRequires: libpython2.3-devel
BuildRequires: libqt3-devel >= 3.2
# BuildRequires: libselinux1-devel
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

Note that Python scripting has been disabled in this package because of
problems with Mandrake 10.0's Boost.Python packaging.  These problems
have been acknowledged and will be fixed for future Mandrake releases,
at which point Regina scripting for Mandrake will be re-enabled.

%prep
%setup -n regina-%{version}

%build
unset QTDIR || : ; . /etc/profile.d/qtdir3.sh
FLAGS="$RPM_OPT_FLAGS -DNDEBUG -DNO_DEBUG"
export CFLAGS="$FLAGS"
export CXXFLAGS="$FLAGS"
./configure --disable-debug --mandir=%{_mandir} --disable-python
make
make check

%install
rm -rf $RPM_BUILD_ROOT
make install-strip DESTDIR=$RPM_BUILD_ROOT

# Delete some huge and unnecessary static libraries.
# rm -f $RPM_BUILD_ROOT%{_libdir}/regina/python/regina.a
rm -f $RPM_BUILD_ROOT%{_libdir}/kde3/libreginapart.a

# Create the Mandrake menu file.
install -m755 -d $RPM_BUILD_ROOT%{_menudir}
cat << EOF > $RPM_BUILD_ROOT%{_menudir}/%{name}
?package(%{name}):command="/usr/bin/regina-kde" \
icon="%{name}.png" needs="X11" \
section="More applications/Sciences/Mathematics" \
title="Regina" longtitle="3-manifold topology software" \
startup_notify="true" mimetypes="application/x-regina,application/x-python" \
accept_url="true" multiple_files="true"
EOF

# Install icons for the Mandrake menu.
install -m644 icons/reginatiny.png -D $RPM_BUILD_ROOT%{_miconsdir}/%{name}.png
install -m644 icons/regina.png -D $RPM_BUILD_ROOT%{_iconsdir}/%{name}.png

%post
/sbin/ldconfig
%{update_menus}

%postun
/sbin/ldconfig
%{clean_menus}

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
%{_bindir}/*
%{_includedir}/regina
%{_libdir}/libregina*
# Make sure we don't ship unwanted static libs by accident.
%{_libdir}/kde3/libreginapart.la
%{_libdir}/kde3/libreginapart.so
# %{_libdir}/regina/python/regina.la
# %{_libdir}/regina/python/regina.so
%{_docdir}/HTML/en/regina
# %{_datadir}/applications/*
%{_datadir}/applnk/*/*
%{_datadir}/apps/regina
%{_datadir}/apps/reginapart
%{_datadir}/icons/*/*/*/*
%{_datadir}/man/*/*
%{_datadir}/mimelnk/*/*
%{_datadir}/regina
%{_datadir}/services/*

# Mandrake-specific files:
%{_menudir}/%{name}
%{_miconsdir}/%{name}.png
%{_iconsdir}/%{name}.png

%changelog
* Sun Jul 25 2004 Ben Burton <bab@debian.org> 4.1.3
- New upstream release.

* Fri Jun 25 2004 Ben Burton <bab@debian.org> 4.1.2
- Initial packaging using Mandrake 10.0 Official.
- Python scripting is initially disabled because of bugs in Mandrake 10.0's
  boost.python packaging (see Mandrake bug #9648).
