# Spec file for PyGMTL.
%define name    pygmtl
%define version	0.5.0
%define release	1

Name: %{name}
Summary: Python bindings for GMTL
Version: %{version}
Release: %{release}
Source: gmtl-%{version}.tar.gz
URL: http://ggt.sourceforge.net/
Group: Development/Python
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
License: LGPL
BuildPrereq: scons >= 0.96.1
BuildPrereq: boost-devel >= 1.32
Obsoletes: pygmtl <= %{version}
Provides: pygmtl = %{version}-%{release}

%description
PyGMTL provides Python language bindings to the Generic Math Template Library
(GMTL), a C++ library. GMTL is a high-performance, extensible, and generic
math library. The design is based upon discussion with many experts in the
field of computer graphics and virtual reality, culminating the efforts of
many previous math library efforts. GMTL gives the graphics programmer
several core math types and a rich library of graphics/math operations on
those types.

%define py_ver %(python -c 'import sys;print(sys.version[0:3])')

%ifarch i386 i486 i586
%define pygmtl_arch i686
%else
%define pygmtl_arch %{_arch}
%endif

%prep
rm -rf %{buildroot}
%setup -q -n gmtl-%{version}

%build
# This needs to be fixed once we have a boost install.
scons EnablePython=True BoostPythonDir=/usr optimize=yes
strip python/gmtl.so

%install
install_dir=`python -c 'import distutils.sysconfig as ds; print ds.get_python_lib(True)'`
mkdir -p %{buildroot}$install_dir
cp -p build.linux-%{pygmtl_arch}/gmtl.so %{buildroot}$install_dir

%clean
rm -rf %{buildroot}

%pre

%post

%preun

%postun

%files
%defattr(-, root, root)
%{_libdir}/python%{py_ver}/site-packages/gmtl.so

%doc AUTHORS ChangeLog COPYING LICENSE.addendum README README-PyGMTL

%changelog
