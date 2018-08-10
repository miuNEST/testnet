# μNEST Core Build Guide

Author:μNEST dev team, http://iotee.io



## Ubuntu 16.04 LTS (64-bit)

Install Dependencies:

```
sudo apt-get install libboost-all-dev libdb++-dev
```

The Boost which ships with 了Ubuntu 16.04 LTS (64-bit) is version 1.58, so it is not necessary to build Boost by yourself and not necessary to set Boost environment variable.

Same steps as those on Ubuntu 14.04 LTS (64-bit):

```
cd ~
git clone https://github.com/miuNEST/testnet.git
cd testnet
git submodule update --init --recursive
cmake  -DCMAKE_BUILD_TYPE=Release .
make -j 4
```

## Windows (64-bit)

On Windows, the final result is witness_node.exe and cli_wallet.exe.

### System Requirements:

1. Windows 64-bit only.
2. Use Visual Studio 2013 Update 5 or Visual Studio 2015 Update 1. Visual Studio 2015 Update 3, Visual Studio2017 is not supported.

Below is an example on Visual Studio 2013 Update 5, with assumption the working directory is c:\bts.

Trivial difference on Visual Studio 2015, they are:

1. Compile command window use VS2015 x64 Native Tools Command Prompt.
2. Warning message "Unknown compiler version" when building Boost 1.57 could be ignored.
3. Build LibCurl、Berkeley DB through *.sln project in VS2015.
4. Choose Visual Studio 14 2015 Win64 in CMake building window.


### Open Visual Studio Command line

Start VS2013 x64 Native Tools Command Prompt in Windows Start Menu. This equals to the following command, by which will open a DOS command line prompt.


```
%comspec% /k ""C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"" amd64
```

All the following commands should be executed in this console until special indication.

### Build OpenSSL

Both OpenSSL 1.0.1 and 1.0.2 work fine, except OpenSSL 1.1.0.


Download OpenSSL source code, e.g. 1.0.2o, and extract it to C:\bts\openssl-1.0.2o.

https://www.openssl.org/source/

https://www.openssl.org/source/openssl-1.0.2o.tar.gz



Install Perl, 32 or 64-bit is fine. Make sure setup the proper PATH environment variable.

https://www.activestate.com/activeperl/downloads


Install NASM for compiling assembly code in OpenSSL. Make sure setup the proper PATH environment variable.

https://www.nasm.us/



Execute following command to compile:

```
set PATH=C:\Program Files\NASM;%PATH%
c:
cd C:\bts\openssl-1.0.2o
perl Configure VC-WIN64A --prefix=C:\bts\openssl-1.0.2o-x64-release-static
ms\do_win64a
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
```

After installation, headers and libs will be copied to C:\bts\openssl-1.0.2o-x64-release-static.

### Build Boost

Boost 1.57 ~ 1.65 should work, while 1.57 is recommended. Since Boost relies on compiler features, more updated version more prone to bug.



Download Boost and extract to c:\bts\boost_1_57_0.

https://sourceforge.net/projects/boost/files/boost/



Execute following command to compile:

```
c:
cd c:\bts\boost_1_57_0
bootstrap
.\b2.exe address-model=64
```

### Download CMake

Download 64-bit version. Suppose to put at c:\bts\cmake-3.11.2-win64-x64.

https://cmake.org/download/

<https://cmake.org/files/v3.11/cmake-3.11.2-win64-x64.zip> 

### Install git

https://git-scm.com/download/win 

### Build LibCurl

https://curl.haxx.se/download.html

<https://curl.haxx.se/download/curl-7.60.0.zip> 

Suppose to put at c:\bts\curl-7.60.0. Open the project c:\bts\curl-7.60.0\projects\Windows\VC12\curl-all.sln with VS2013. Build the configuration with DLL Release + x64 tag.

The result is:

    c:\bts\curl-7.60.0\build\Win64\VC12\DLL Release\libcurl.lib

    c:\bts\curl-7.60.0\build\Win64\VC12\DLL Release\libcurl.dll

Put the directory in PATH environment variable, or copy ibcurl.dll to the same directory with witness_node.exe or cli_wallet.exe.

### Build Berkeley DB

https://www.oracle.com/technetwork/database/database-technologies/berkeleydb/downloads/index.html

http://download.oracle.com/otn/berkeley-db/db-18.1.25.zip

Suppose to put at c:\bts\db-18.1.25. Open the project c:\bts\db-18.1.25\build_windows\Berkeley_DB_vs2012.sln with VS2013.
The build configuration should be the same as bitshares-core's. For example, Debug + x64 for debug version and Release + 64 for release.

The result for a Debug version is:

    Header File: c:\bts\db-18.1.25\build_windows

    Lib File: c:\bts\db-18.1.25\build_windows\x64\Debug

Set PATH environment variable to include the directory of libdb181d.dll, and rename libdb181d.lib to db_cxx.lib.

### Download Doxygen

Optional, not supported yet.

### Download μNEST Core Source Code


```
c:
cd c:\bts
git clone https://github.com/miuNEST/testnet.git
cd testnet
git submodule update --init --recursive
```

### Create Build Script

The content of c:\bts\setenv_x64.bat, which are used by CMake to locate libraries:

```
@echo off
set GRA_ROOT=c:\bts
set OPENSSL_ROOT=%GRA_ROOT%\openssl-1.0.2o-x64-release-static
set OPENSSL_ROOT_DIR=%OPENSSL_ROOT%
set OPENSSL_INCLUDE_DIR=%OPENSSL_ROOT%\include
set BOOST_ROOT=%GRA_ROOT%\boost_1_57_0
set CMAKE_ROOT=%GRA_ROOT%\cmake-3.11.2-win64-x64
set PATH=%BOOST_ROOT%\lib;%CMAKE_ROOT%\BIN;%PATH%
set CURL_INCLUDE_DIR=%GRA_ROOT%\curl-7.60.0\include
set CURL_LIB_DIR=%GRA_ROOT%\curl-7.60.0\build\Win64\VC12\DLL Release
set BDB_INCLUDE_DIR=%GRA_ROOT%\db-18.1.25\build_windows
set BDB_LIB_DIR=%GRA_ROOT%\db-18.1.25\build_windows\x64\Debug
```

### Run CMake

Open CMake UI with following command:

```
c:
cd C:\bts
setenv_x64.bat
cmake-gui
```

In the CMake interface:

Set to c:/bts/bitshares-core in "Where is source code"，
Set to c:/bts/bin in "Where to build binaries", click "Configure" and choose Visual Studio 12 2013 Win64 in the pop up menu.
Since we doesn't build Debug version of OpenSSL, we need to set LIB_EAY_DEBUG,SSL_EAY_DEBUG for CMake to setup properly.
Then click Generate to generate VS2013 project file.

Tips:

Start VS2013 in the same console as we run setenb_x64.bat. By doing so, the environment variables are inherited. Otherwise, VS2013 will report error during updating the project by CMake, which says dependencies like LibCurl and Berkeley DB are not found.

```
start "" "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.exe"
```

Or

Set all the environment variables as Windows Global variable, so that it would be not necessary to start VS2013 from the console.

### Build μNEST Core

Open c:\bts\bin\testnet.sln with VS2013, choose witness_node, cli_wallet target to build and you will get witness_node.exe、cli_wallet.exe.

In case error like msvcp120.dll, msvcr120.dll not found are encountered when you deploy witness_node.exe/cli_wallet.exe to another machine, two options to solve it:

1. Modify project property to Use MFC in a Static Library and recompile.
2. Install Visual C++ Redistributable Packages for Visual Studio 2013(https://www.microsoft.com/en-us/download/details.aspx?id=40784) on target machine. You could find it on local machine at C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\redist\x64.

## OS X

### Install XCode

Install in macOS App Store. Refer to <https://guide.macports.org/#installing.xcode>.

### Install Homebrew

Run the following command in macOS terminal. Refer to <https://brew.sh/>.

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

### Install Dependencies

Boost available version 1.57~1.65, OpenSSL available versiono 1.0.1,1.0.2.

```
brew doctor
brew update
brew install boost boost@1.57 cmake git openssl autoconf automake berkeley-db libtool
brew link --force openssl
```

After installation, run this command in macOS terminal "ls -l /usr/local/opt/boost" to show /usr/local/opt/boost is linked to which Boost version.
The same as /usr/local/opt/openssl. 
In cse they are not the version we required, we can use Boost, OpenSSL directory with specific version number in CMake command line.

### Build μNEST Core

```
cd ~
git clone https://github.com/miuNEST/testnet.git
cd testnet
git submodule update --init --recursive
cmake -DBOOST_ROOT=/usr/local/opt/boost@1.57 -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl .
make -j 4
```




## Setup testnet

Refer to bitshares official doc: http://docs.bitshares.org/testnet/private-testnet.html



## Reference

https://github.com/bitshares/bitshares-core/wiki/BUILD_UBUNTU

https://github.com/bitshares/bitshares-core/wiki/BUILD_WIN32

https://github.com/bitshares/bitshares-core/wiki/Building-on-OS-X

[https://github.com/abitmore/bts-cn-docs/blob/master/使用VisualStudio2015编译BitShares-Core.txt](https://github.com/abitmore/bts-cn-docs/blob/master/使用VisualStudio2015编译BitShares-Core.txt)

