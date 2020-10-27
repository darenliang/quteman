# Quteman

A simple Postman alternative written in Qt.

### Building

Make sure you have the following tools installed before building.

* git
* python3 with venv and pip
* gcc or clang with libstdc++11 support

#### Initialize project:

```
git clone https://github.com/darenliang/quteman.git
cd quteman
git submodule update --init
```

#### Build:

```
./bootstrap.sh
```

Note that you may be prompted to enter a password to install any required system packages.

#### Run:

```
./build/src/bin/quteman
```

#### Run AppImage:

```
./build/Quteman-x86_64.AppImage
```

Your AppImage name may include a hash like this: `Quteman-61243a3-x86_64.AppImage`.

