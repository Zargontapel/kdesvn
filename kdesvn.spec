# This spec file was generated using Kpp
# If you find any problems with this spec file please report
# the error to ian geiser <geiseri@msoe.edu>
Summary:   A subversion client for the KDE with KIO integration.
Name:      kdesvn
Version:   0.8.0
Release:   1%{?dist}
License:   LGPL
Vendor:    Rajko Albrecht <ral@alwins-world.de>
Url:       http://www.alwins-world.de
Packager:  Rajko Albrecht <ral@alwins-world.de>
Group:     Development/Tools
Source:    kdesvn-%version.tar.gz
BuildRoot: %{_tmppath}/%{name}-root
Requires: kdesvn-svnqt = %{version}
Obsoletes: kdesvn-kioksvn

%description
Kdesvn is a subversion client for KDE.
It may used as standalone application or plugin (KPart). Base functions are provided
via a KIO protocoll, too.

%package kiosvn
Requires: kdesvn = %{version}
Group:    Development/Tools
Summary:  A kde-kio integration for subversion based on kdesvn

%description kiosvn
KIO integration (KIO::svn) based on kdesvn alternative protocol name.

%package svnqt
Group:    System Environment/Libraries
Summary:  Wrapper lib for subversion QT integration.
Requires: subversion >= 1.2.0
BuildPreReq: apr-devel
BuildPreReq: apr-util-devel
BuildPreReq: neon-devel
BuildPreReq: subversion-devel >= 1.2.0

%description svnqt
Shared lib which contains a QT C++ wrapper for subversion. It is core part
of kdesvn but is designed to not require KDE so plain QT programs may use
it.

%package svnqt-devel
Group:    Development/Libraries
Summary:  Wrapper lib for subversion QT integration.
Requires: subversion-devel >= 1.2.0
Requires: kdesvn-svnqt = %{version}

%description svnqt-devel
Development files for kdesvn-svnqt

%prep
%setup
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" ./configure \
                --prefix=`kde-config --prefix` \
                --disable-no-exceptions \
                --disable-debug
%build
# Setup for parallel builds
numprocs=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "$numprocs" = "0" ]; then
  numprocs=1
fi

make -j$numprocs

%install
make install-strip DESTDIR=$RPM_BUILD_ROOT

cd $RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT/*
rm -rf $RPM_BUILD_DIR/kdesvn


%files
%{_bindir}/kdesvn
%{_bindir}/kdesvnaskpass
%{_libdir}/*/*part*
%{_datadir}/applications/*
%{_datadir}/apps/kdesvn/*
%{_datadir}/apps/kdesvnpart/*
%{_datadir}/config.kcfg/*
%{_datadir}/doc/*
%{_datadir}/icons/*
%{_datadir}/locale/*
%{_libdir}/*/kded_kdesvnd.*
%{_datadir}/services/kded/kdesvnd.desktop
%{_datadir}/apps/konqueror/servicemenus/kdesvn*
%{_libdir}/*/kio_ksvn*
%{_datadir}/services/ksvn*.protocol

%files kiosvn
%{_datadir}/services/svn*.protocol

%files svnqt
%{_libdir}/libsvnqt*.so*

%files svnqt-devel
%{_libdir}/libsvnqt.la
%{_includedir}/svnqt/*

