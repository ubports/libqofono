# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.26
# 

Name:       libqofono

# >> macros
# << macros

Summary:    A library of Qt bindings for ofono
Version:    0.29
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
URL:        https://github.com/nemomobile/libqofono
Source0:    libqofono-%{version}.tar.bz2
Source100:  libqofono.yaml
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(QtCore)
BuildRequires:  pkgconfig(QtDBus)
BuildRequires:  pkgconfig(mlite)

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

%description tests
This package contains qml test for ofono Qt bindings.

%prep
%setup -q -n libqofono-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qmake  \
    CONFIG+=mlite

make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake_install

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
%{_libdir}/qt4/imports/MeeGo/QOfono/*
# >> files declarative
# << files declarative

%files devel
%defattr(-,root,root,-)
%{_libdir}/%{name}.prl
%{_libdir}/%{name}.so
%{_libdir}/pkgconfig/qofono.pc
%{_includedir}/qofono/*.h
%{_includedir}/qofono/dbus/ofono*.h
%{_includedir}/qofono/dbus/ofono*.xml
%{_datadir}/qt4/mkspecs/features/qofono.prf
# >> files devel
# << files devel

%files tests
%defattr(-,root,root,-)
/opt/ofonotest/bin/ofonotest
/opt/ofonotest/qml/ofonotest/*.qml
%{_libdir}/%{name}/tests/*
# >> files tests
# << files tests
