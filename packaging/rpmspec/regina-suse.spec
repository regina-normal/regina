# Known to work for:
# - SuSE 11.0 (i586, x86_64)
# - SuSE 10.3 (i586, x86_64)
# - SuSE 10.2 (i586, x86_64)

Name: regina-normal
Summary: 3-manifold topology software with normal surface support
Version: 4.5.1
Release: 1.%{_vendor}
License: GPL
# I wish there were a more sane group (like Applications/Mathematics).
Group: Applications/Engineering
Source: http://prdownloads.sourceforge.net/regina/regina-%{version}.tar.gz
URL: http://regina.sourceforge.net/
Packager: Ben Burton <bab@debian.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

Requires: kdelibs3
Requires: kdebase3
Requires: kdegraphics3-pdf
Requires: python
Requires: susehelp
Conflicts: regina

BuildConflicts: regina-normal < %{version}
BuildConflicts: regina-normal > %{version}
BuildRequires: boost
%if 0%{?suse_version} == 1100
# The boost.python originally shipped with SuSE 11.0 is broken (bug #401964).
# Insist on a patched boost from the updates repository.
BuildRequires: boost-devel >= 1.34.1-42.2
%else
BuildRequires: boost-devel
%endif
BuildRequires: cppunit
BuildRequires: cppunit-devel
BuildRequires: doxygen
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: gmp
BuildRequires: gmp-devel
BuildRequires: kdelibs3-devel >= 3.2
BuildRequires: libjpeg-devel
BuildRequires: libstdc++-devel
BuildRequires: libxml2-devel
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

%build
FLAGS="$RPM_OPT_FLAGS -DNDEBUG -DNO_DEBUG"
export CFLAGS="$FLAGS"
export CXXFLAGS="$FLAGS"
./configure \
%if "%{_lib}" != "lib"
  --enable-libsuffix="%(A=%{_lib}; echo ${A/lib/})" \
%endif
  --disable-mpi --disable-debug --includedir=%{_includedir} --mandir=%{_mandir} --with-python-version=2.5

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
