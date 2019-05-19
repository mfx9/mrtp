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

Make sure you have the necessary libraries installed. You may want to review 
the makefile. Finally, run make in the main directory. The executable 
should appear in bin/mrtp\_cli. 

```
apt-get install libpng-dev libpng++-dev libeigen3-dev
git submodule update --init --recursive
mkdir bin build
make
```

### Gallery

<img src="./sample.png" alt="Sample image" width="400" />
