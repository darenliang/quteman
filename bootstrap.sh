#!/bin/bash

# Exit on error
set -e

# Create/activate venv
if [ -d "venv" ]; then
  echo "Virtual environment found."
else
  echo "Python virtual environment not created, creating one right now..."
  python3 -m venv venv
fi
source venv/bin/activate

# Install conan
if pip list | grep -i "conan"; then
  echo "Conan found."
else
  echo "Conan not found. Installing it..."
  pip install conan
fi

# Install cmake
if pip list | grep -i "cmake"; then
  echo "Cmake found."
else
  echo "Cmake not found. Installing it..."
  pip install cmake
fi

REMOTE="https://api.bintray.com/conan/bincrafters/public-conan"

# Add bintray remote
if conan remote list | grep -i "$REMOTE"; then
  echo "Bintray remote found."
else
  echo "Bintray remote not found. Adding it..."
  conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
fi

# Build deps
cd conan
export CONAN_SYSREQUIRES_MODE="enabled"
conan install . -s compiler.libcxx=libstdc++11 --build=missing
cd ../

# Make build folder
if [ -d "build" ]; then
  echo "Build folder found."
else
  echo "Build folder not found. Creating it..."
  mkdir build
fi

# Build project
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make -j$(nproc)
make install DESTDIR=AppDir

# Build AppImage
wget -N https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget -N https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x linuxdeploy*.AppImage
./linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage
