Name:           xbtt
Version:        0.2.9
Release:        1%{?dist}
Summary:        XBT Torrents ReTracker

License:        GPLv3
URL:            https://github.com/LanetNetwork/xbtt
Source0:        xbtt-0.2.9.tar.gz

BuildRequires:  gcc-c++ cmake make mariadb-devel openssl-devel gperftools-devel
Requires:       mariadb-libs openssl-libs gperftools-devel

%description
XBT Torrents ReTracker

%prep
%setup -q

%build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr ..
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%{__mkdir_p} %{buildroot}/etc/%{name}
%{__mkdir_p} %{buildroot}/usr/lib/systemd/system/
%{__install} -m0644 configs/%{name}.conf.sample %{buildroot}/etc/%{name}
%{__install} -m0644 configs/%{name}.service %{buildroot}/usr/lib/systemd/system/%{name}.service
cd build
make install

%files
%doc COPYING README.md
/usr/bin/%{name}
/etc/%{name}/%{name}.conf.sample
/usr/lib/systemd/system/%{name}.service

%changelog
