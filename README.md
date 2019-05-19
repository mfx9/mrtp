# Mrtp

A weekend project to build a simple raytracer in C++. 

Textures are from: https://opengameart.org/content/100-seamless-textures

Mrtp depends on the following libraries and tools (Debian packages):
 * libpng-dev
 * libpng++-dev
 * libeigen3-dev

Mrtp also depends on (submodules): 
 * cpptoml

### Installation

You may start from reviewing the makefile. Then, run the following 
commands. The executable should be in bin/mrtp\_cli. 

```
git submodule update --init --recursive
mkdir bin build
make
```

### Gallery

<img src="./sample.png" alt="Sample image" width="400" />
