Name:           udisks2-qt5
Version:        5.0.5
Release:        1%{?dist}
Summary:        Qt5 binding for udisks2
License:        GPLv3+
URL:            https://github.com/linuxdeepin/udisks2-qt5
%if 0%{?fedora}
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz
%else
Source0:        %{name}_%{version}.tar.gz
%endif
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig(Qt5Core)

%description
This package provides a Qt5 binding for udisks2.

%package devel
Summary:        Development package for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       qt5-qtbase-devel%{?isa}

%description devel
Header files and libraries for %{name}.

%prep
%autosetup -p1
sed -i 's|/lib|/%{_lib}|' udisks2.pro

%build
# help find (and prefer) qt5 utilities, e.g. qmake, lrelease
export PATH=%{_qt5_bindir}:$PATH
%qmake_qt5 PREFIX=%{_prefix}
%make_build

%install
%make_install INSTALL_ROOT=%{buildroot}

%files
%doc CHANGELOG.md
%{_libdir}/lib%{name}.so.0*

%files devel
%{_includedir}/%{name}
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc

%changelog
