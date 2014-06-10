#
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
#

Name:       libqofono-qt5

# >> macros
# << macros

Summary:    A library of Qt 5 bindings for ofono
Version:    0.53
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
URL:        https://github.com/nemomobile/libqofono
Source0:    %{name}-%{version}.tar.bz2
Source100:  libqofono-qt5.yaml
Requires:   qt5-qtcore
Requires:   mobile-broadband-provider-info
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(Qt5XmlPatterns)

%description
This package contains Qt bindings for ofono cellular service
interfaces.


%package declarative
Summary:    Declarative plugin for libqofono
Group:      Development/Tools
Requires:   %{name} = %{version}-%{release}
Requires:   %{name} = %{version}

%description declarative
This package contains declarative plugin for libofono.

%package devel
Summary:    Development files for ofono Qt bindings
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   %{name} = %{version}

%description devel
This package contains the development header files for the ofono Qt bindings.

%package tests
Summary:    qml test app for the ofono Qt bindings
Group:      Development/Tools
Requires:   %{name} = %{version}-%{release}
Requires:   blts-tools

%description tests
This package contains qml test for ofono Qt bindings.

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
export QT_SELECT=5
# << build pre

%qmake5

make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
export QT_SELECT=5
# << install pre
%qmake5_install

# >> install post
# << install post

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/%{name}.so.*
# >> files
# << files

%files declarative
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/MeeGo/QOfono/*
# >> files declarative
# << files declarative

%files devel
%defattr(-,root,root,-)
%{_libdir}/%{name}.prl
%{_libdir}/%{name}.so
%{_libdir}/pkgconfig/qofono-qt5.pc
%{_includedir}/qofono-qt5/*.h
%{_includedir}/qofono-qt5/dbus/ofono*.h
%{_includedir}/qofono-qt5/dbus/ofono*.xml
%{_datadir}/qt5/mkspecs/features/qofono-qt5.prf
# >> files devel
# << files devel

%files tests
%defattr(-,root,root,-)
%{_libdir}/%{name}/tests/*
/opt/tests/%{name}/*
# >> files tests
# << files tests
