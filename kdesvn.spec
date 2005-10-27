# This spec file was generated using Kpp
# If you find any problems with this spec file please report
# the error to ian geiser <geiseri@msoe.edu>
Summary:   A subversion client for the KDE
Name:      kdesvn
Version:   0.7.0
Release:   1%{?dist}
License:   LGPL
Vendor:    Rajko Albrecht <ral@alwins-world.de>
Url:       http://www.alwins-world.de
Icon:      kdesvn_logo_klein.xpm
Packager:  Rajko Albrecht <ral@alwins-world.de>
Group:     Development/Tools
Source:    kdesvn-%version.tar.gz
BuildRoot: %{_tmppath}/%{name}-root
BuildPreReq: apr-devel
BuildPreReq: apr-util-devel
BuildPreReq: neon-devel
BuildPreReq: subversion-devel >= 1.1.0
Requires: subversion >= 1.1.0

%description
Kdesvn is a subversion client for KDE.

%package kioksvn
Requires: kdesvn = %{version}
Group:    Development/Tools
Summary:  A kde-kio integration for subversion based on kdesvn

%description kioksvn
KIO integration (KIO::ksvn) based on kdesvn

%package kiosvn
Requires: kdesvn-kioksvn = %{version}
Group:    Development/Tools
Summary:  A kde-kio integration for subversion based on kdesvn

%description kiosvn
KIO integration (KIO::svn) based on kdesvn alternative protocol name.

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
%{_datadir}/apps/*
%{_datadir}/config.kcfg/*
%{_datadir}/doc/*
%{_datadir}/icons/*
%{_datadir}/locale/*

%files kioksvn
%{_bindir}/kdesvnd
%{_libdir}/*/kio_ksvn*
%{_datadir}/services/ksvn*.protocol
%{_datadir}/services/kdesvnd.desktop

%files kiosvn
%{_datadir}/services/svn*.protocol
