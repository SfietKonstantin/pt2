Name:       libpt2
Summary:    A library that manages transportation informations
Version:    0.0.0
Release:    1
Group:      Applications/Internet
License:    BSD
URL:        https://github.com/sfietkonstantin/pt2
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  qt5-qttools
BuildRequires:  qt5-qttools-linguist

%description
TODO

#%package qml-plugin-ts-devel
#Summary:   Translation source for libsocialcache
#License:   BSD
#Group:     Applications/Internet

#%description qml-plugin-ts-devel
#Translation source for socialcache qml plugin

%package devel
Summary:   Development files for libpt2
License:   BSD
Group:     Development/Libraries
Requires:  libpt2 = %{version}

%description devel
This package contains development files for libpt2.

%package qml-plugin
Summary:   QML plugin for libpt2
License:   BSD
Group:     Applications/Internet

%description qml-plugin
This package contains the qml plugin for libpt2

%package plugin-test
Summary:   Test plugin for libpt2
License:   BSD
Group:     Applications/Internet

%description plugin-test
This package contains the test plugin for libpt2

%package plugin-ratp
Summary:   RATP plugin for libpt2
License:   BSD
Group:     Applications/Internet

%description plugin-ratp
This package contains the RATP plugin for libpt2

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5 CONFIG+=semistatic
make %{_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libpt2.so.*
%{_bindir}/pt2-provider

#%files qml-plugin-ts-devel
#%defattr(-,root,root,-)
#%{_datadir}/translations/source/pt2.ts

%files devel
%defattr(-,root,root,-)
%{_includedir}/pt2/*.h
%{_includedir}/pt2/base/*.h
%{_includedir}/pt2/dbus/*.h
%{_includedir}/pt2/manager/*.h
%{_includedir}/pt2/provider/*.h
%{_libdir}/libpt2.so
%{_libdir}/pkgconfig/pt2.pc

%files qml-plugin
%{_libdir}/qt5/qml/org/SfietKonstantin/pt2/qmldir
%{_libdir}/qt5/qml/org/SfietKonstantin/pt2/libpt2qml.so
#%{_datadir}/translations/pt2_eng_en.qm

%files plugin-test
/usr/share/pt2/plugins/libtest.so
/usr/share/pt2/plugins/test.desktop

%files plugin-ratp
/usr/share/pt2/plugins/libratp.so
/usr/share/pt2/plugins/ratp.desktop
/usr/share/pt2/plugins/ratp/ratp.db
