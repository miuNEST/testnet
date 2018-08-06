# μNEST的BitShares Core编译说明

作者：μNEST开发组，http://iotee.io



## Ubuntu 16.04 LTS (64-bit)

### 安装依赖的包：

```
sudo apt-get update
sudo apt-get install libboost-all-dev libdb++-dev doxygen
```

### 编译μNEST的BitShares Core

```
cd ~
git clone https://github.com/miuNEST/testnet.git
cd testnet
git submodule update --init --recursive
cmake  -DCMAKE_BUILD_TYPE=Release .
make -j 4
```

## Windows (64-bit)

Windows环境下，最终是要编译出witness_node.exe、cli_wallet.exe这两个程序。

### 环境要求：

1. Windows系统须为64位。
2. 编译器请使用Visual Studio 2013 Update 5或者Visual Studio 2015 Update 1。目前不支持用Visual Studio 2015 Update 3、Visual Studio 2017编译。

以下以VS2013 Update 5为例，并假定顶层工作目录为c:\bts。


VS2015 Update 1的编译与VS2013略有差异，具体为：

1. 编译的命令行窗口采用VS2015 x64 Native Tools Command Prompt。
2. 编译Boost 1.57时有警告信息“Unknown compiler version”，忽略即可。
3. 编译LibCurl、Berkeley DB时打开针对VS2015的*.sln工程。
4. CMake对话框中要选择Visual Studio 14 2015 Win64。

### 打开Visual Studio的命令行窗口

从Windows开始菜单中点击快捷方式VS2013 x64 Native Tools Command Prompt，这个快捷方式实际执行的是如下命令，该命令会打开一个DOS命令行窗口。以下如无特殊指明，我们均在此命令行窗口中执行后续所有编译命令行。

```
%comspec% /k ""C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"" amd64
```

### 编译OpenSSL

可以用OpenSSL 1.0.1或者1.0.2，不能用OpenSSL 1.1.0。

下载并解压OpenSSL源码。以1.0.2o为例，假定解压到C:\bts\openssl-1.0.2o目录。
https://www.openssl.org/source/
https://www.openssl.org/source/openssl-1.0.2o.tar.gz


安装Perl解释器。32或者64位的均可。需要保证将perl.exe的所在目录添加到PATH环境变量中。
https://www.activestate.com/activeperl/downloads


安装NASM汇编器。用来编译OpenSSL加速用的汇编代码。需要将nasm.exe的所在目录加入到PATH环境变量中。
https://www.nasm.us/


执行编译命令：

```
set PATH=C:\Program Files\NASM;%PATH%
c:
cd C:\bts\openssl-1.0.2o
perl Configure VC-WIN64A --prefix=C:\bts\openssl-1.0.2o-x64-release-static
ms\do_win64a
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
```

编译好的头文件和lib会自动拷贝到C:\bts\openssl-1.0.2o-x64-release-static目录下。

### 编译Boost

Boost版本可以是1.57 ~ 1.65，推荐用1.57。由于Boost使用了很多依赖编译器的特性，所以越新的Boost版本越容易有编译问题。

下载Boost。假定解压得到的目录为c:\bts\boost_1_57_0。
https://sourceforge.net/projects/boost/files/boost/


执行编译命令：

```
c:
cd c:\bts\boost_1_57_0
bootstrap
.\b2.exe address-model=64
```

### 下载CMake

下载64位版本。假定将CMake解压在c:\bts\cmake-3.11.2-win64-x64目录。
https://cmake.org/download/
<https://cmake.org/files/v3.11/cmake-3.11.2-win64-x64.zip> 

### 下载安装git

https://git-scm.com/download/win 

### 编译LibCurl

https://curl.haxx.se/download.html
<https://curl.haxx.se/download/curl-7.60.0.zip> 

假定解压在c:\bts\curl-7.60.0目录。用VS2013的IDE打开工程c:\bts\curl-7.60.0\projects\Windows\VC12\curl-all.sln，编译DLL Release + x64这个配置。

编译产物是：
​    c:\bts\curl-7.60.0\build\Win64\VC12\DLL Release\libcurl.lib
​    c:\bts\curl-7.60.0\build\Win64\VC12\DLL Release\libcurl.dll

最后要将libcurl.dll所在目录加入到PATH环境变量中，或者将libcurl.dll拷贝到witness_node.exe、cli_wallet.exe的所在目录。

### 编译Berkeley DB

https://www.oracle.com/technetwork/database/database-technologies/berkeleydb/downloads/index.html
http://download.oracle.com/otn/berkeley-db/db-18.1.25.zip

假定解压在c:\bts\db-18.1.25目录。用VS2013的IDE打开工程c:\bts\db-18.1.25\build_windows\Berkeley_DB_vs2012.sln，编译对应配置的版本，注意Berkeley DB的编译配置要和witness_node.exe、cli_wallet.exe的编译配置一致，此处假定是编译的Debug + x64这个配置的版本，用于调试。如果用于发布，请编译Release版本。

编译产物：
​    头文件在c:\bts\db-18.1.25\build_windows
​    lib和dll在c:\bts\db-18.1.25\build_windows\x64\Debug
请将libdb181d.dll的所在路径加入PATH环境变量中，并将libdb181d.lib改名或复制为db_cxx.lib。

### 下载 Doxygen

可选。暂不支持生成Doxygen文档。

### 下载μNEST的BitShares Core源码

```
c:
cd c:\bts
git clone https://github.com/miuNEST/testnet.git
cd testnet
git submodule update --init --recursive
```

### 建立批处理文件

新建批处理文件c:\bts\setenv_x64.bat ，内容如下。这个批处理设置的环境变量都是给CMake用来定位各个库的。

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

### 运行CMake

运行如下命令打开CMake的界面：

```
c:
cd C:\bts
setenv_x64.bat
cmake-gui
```

CMake界面上，Where is source code选择c:/bts/bitshares-core，Where to build binaries选择c:/bts/bin，点击Configure按钮，在弹出来的对话框中选择Visual Studio 12 2013 Win64。

由于我们并未编译OpenSSL的Debug版本，所以CMake无法自动设置LIB_EAY_DEBUG、SSL_EAY_DEBUG的值，只需要手动将其分别设置为LIB_EAY_RELEASE、SSL_EAY_RELEASE的值即可。

然后点击Generate按钮，等生成VS2013的工程文件完毕。

在运行setenv_x64.bat的VS2013 x64 Native Tools Command Prompt命令行窗口中，运行如下的命令来打开VS2013的IDE，这样VS2013的IDE进程会自动继承setenv_x64.bat中设置好的环境变量，避免后续VS2013在按需调用CMake刷新工程时找不到我们提供的LibCurl、Berkeley DB等库的路径时报错。

```
start "" "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.exe"
```

还有个办法是将setenv_x64.bat中的环境变量统一设置为Windows的系统全局环境变量，这样无需每次从运行setenv_x64.bat的命令行窗口中来启动VS2013的devenv.exe。

### 编译μNEST的BitShares Core

用VS2013的IDE打开工程c:\bts\bin\BitShares.sln，右键分别选择witness_node、cli_wallet这两个工程编译，得到witness_node.exe、cli_wallet.exe。

如果将witness_node.exe、cli_wallet.exe部署到别的机器上时出现找不到msvcp120.dll、msvcr120.dll之类的错误，有两种办法解决：

1. 修改witness_node、cli_wallet这两个工程的属性，设置为Use MFC in a Static Library，然后重新编译。
2. 在部署的目标机上安装Visual C++ Redistributable Packages for Visual Studio 2013：https://www.microsoft.com/en-us/download/details.aspx?id=40784。这些文件实际上在编译机的如下位置也能找到：C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\redist\x64。

## OS X

### 安装XCode

在macOS的App Store中安装。参考：<https://guide.macports.org/#installing.xcode>。

### 安装Homebrew包管理器

在macOS的terminal窗口中执行如下命令安装。参考：https://brew.sh/。

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

### 安装依赖的包

Boost只能用1.57~1.65版本，OpenSSL只能用1.0.1、1.0.2的版本。

```
brew doctor
brew update
brew install boost boost@1.57 cmake git openssl autoconf automake berkeley-db libtool
brew link --force openssl
```

安装后，在macOS的terminal窗口中执行命令ls -l /usr/local/opt/boost，能看到符号链接/usr/local/opt/boost具体指向的是Boost的哪个版本。符号链接/usr/local/opt/openssl也类似。如果这两个符号链接指向的不是我们所要求的版本，则可以在CMake的命令行中不使用这两个符号链接，而是使用带具体版本号的的Boost、OpenSSL目录。

### 编译μNEST的BitShares Core源码

```
cd ~
git clone https://github.com/miuNEST/testnet.git
cd testnet
git submodule update --init --recursive
cmake -DBOOST_ROOT=/usr/local/opt/boost@1.57 -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl .
make -j 4
```

## 搭建testnet

请参考bitshares的文档：http://docs.bitshares.org/testnet/private-testnet.html



## 参考资料

https://github.com/bitshares/bitshares-core/wiki/BUILD_UBUNTU

https://github.com/bitshares/bitshares-core/wiki/BUILD_WIN32

https://github.com/bitshares/bitshares-core/wiki/Building-on-OS-X

[https://github.com/abitmore/bts-cn-docs/blob/master/使用VisualStudio2015编译BitShares-Core.txt](https://github.com/abitmore/bts-cn-docs/blob/master/使用VisualStudio2015编译BitShares-Core.txt)

