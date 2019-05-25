# Mrtp

A weekend project to build a simple raytracer in C++. 

Textures are from: https://opengameart.org/content/100-seamless-textures

Mrtp depends on the following libraries:
 * libpng-dev
 * libpng++-dev
 * libeigen3-dev
 * cpptoml

### Installation

Make sure you have the necessary libraries installed. Otherwise, try (in Debian):

```
apt-get install libpng-dev libpng++-dev libeigen3-dev
```

You may want to review the makefile. Run make in the main directory. The executable 
should appear in bin/mrtp\_cli. 

```
git submodule update --init --recursive
mkdir bin build
make
```

In order to test the program, run from inside the examples directory:

```
../bin/mrtp_cli scene*toml
```

### Gallery

<img src="./sample.png" alt="Sample image" width="400" />
