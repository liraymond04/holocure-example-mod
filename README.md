# HoloCure Example Mod

A mod template for HoloCure using [YYToolkit](https://github.com/Archie-osu/YYToolkit)

This GitHub repository is configured with GitHub actions to automatically build a mod DLL and upload it to releases when a commit is made to the `main` branch

The mod DLL name and version are defined inside the `Dockerfile` using `DLL_NAME` and `VERSION` at the top of the file (WARNING: do not edit the rest of the Dockerfile unless you know what you are doing)

Releases published by GitHub Actions are tagged with the `VERSION`, and the changelog of the release is defined inside of [CHANGELOG.md](CHANGELOG.md)

## Building with Docker

Building a mod DLL is simplified with [Docker](https://github.com/microsoft/docker), and it only requires that you have Docker and Docker's Buildkit installed

For Arch Linux, you can install Docker and Buildkit with pacman

```bash
sudo pacman -S docker docker-buildx
```

Building with Docker will require an internet connection and ~8gb of disk space in Docker's root directory

The DLL name and version can be edited in `Dockerfile` at the top of the file (WARNING: do not edit the rest of the Dockerfile unless you know what you are doing)

Start the build with Docker by running `build.sh`
```bash
./build.sh
```

DLL will be outputted to `build/example-mod-v1.0.0.dll`

## Building with CMake

### Linux

Building a mod DLL through CMake requires using [msvc-wine](https://github.com/mstorsjo/msvc-wine) 

For Arch Linux, the prerequisite packages are,

```bash
sudo pacman -S --needed git gcc make cmake wine msitools samba python python-simplejson python-six
```

To install the MSVC compiler, we can clone the repository and run the install scripts

```bash
git clone https://github.com/mstorsjo/msvc-wine.git
cd msvc-wine

# This example installs the compiler to ~/my_msvc/opt/msvc
./vsdownload.py --dest ~/my_msvc/opt/msvc
./install.sh ~/my_msvc/opt/msvc

# Add compiler commands to PATH
export PATH=~/my_msvc/opt/msvc/bin/x64:$PATH

# Optional: Start a persistent wineserver
wineserver -k # Kill a potential old server
wineserver -p # Start a new server
wine64 wineboot # Run a process to start up all background wine processes
```

Generate build files and compile
```bash
mkdir build && cd build
CC=cl CXX=cl cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Windows
make
```

DLL will be outputted to `x64/Release/Src.dll`

#### (Optional) Fix compile_commands.json
If you are using clangd for Intellisense, you will need to edit the generated `compile_commands.json` to replace the MSVC compiler with the mingw-w64 cross compiler

For Arch Linux, you need to install the mingw-w64-gcc package

```bash
sudo pacman -S mingw-w64-gcc
```

Create a `.secrets` file and add the paths for the old and new compilers
```bash
# Example .secrets file

# path to MSVC compiler cl
old_compiler="/home/liray/my_msvc/opt/msvc/bin/x64/cl"
# path to mingw-w64-g++
new_compiler="/usr/bin/x86_64-w64-mingw32-g++"
```

Run the `update_compile_commands.sh` script and pass in the `.secrets` and optionally the `compile_commands.json` (if the `compile_commands.json` is not passed in, it is searched for in the directory the script is run in) 

```bash
./update_compile_commands.sh -f ./secrets -j build/compile_commands.json
```


