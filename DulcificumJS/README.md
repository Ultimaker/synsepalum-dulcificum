# Dulcificum JS

A worker that runs Dulcificum in a browser using the Emscripten to compile for wasm

## System Requirements

### Windows
- Python 3.11 or higher
- Ninja 1.10 or higher
- VS2022 or higher
- CMake 3.23 or higher
- nmake

### MacOs
- Python 3.11 or higher
- Ninja 1.10 or higher
- apply clang 11 or higher
- CMake 3.23 or higher
- make
-
### Linux
- Python 3.11 or higher
- Ninja 1.10 or higher
- gcc 13 or higher
- CMake 3.23 or higher
- make

# Installation
We are using conan to manage our C++ dependencies and build configuration. If you have never used Conan read their documentation which is quite extensive and well maintained.

1. Configure Conan

2. Before you start, if you use conan for other (big) projects as well, it's a good idea to either switch conan-home and/or backup your existing conan configuration(s).

That said, installing our config goes as follows:

```
pip install conan==1.64
conan config install https://github.com/ultimaker/conan-config.git
conan profile new default --detect --force
```

2. Installing NodeJS deps

Using Dulcificum in this project follows normal NodeJS package conventions. During an explicit user instigated pre-install step,
we use Conan (installed and setup above), to build or download the wasm binaries from the JFrog Artifactory.
and installing these in `src/Dulcificum/`

```
npm run install_dulcificum
npm install
```
