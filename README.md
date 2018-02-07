# Mrtp
<img src="./sample.png" alt="Sample image" width="400" />


A simple raytracing tool to explore programming in C++.<br>
Released as open source software under the GNU GPL license (see COPYING).

Mrtp builds up on the idea of my earlier program, mrt, that I wrote back in 
2005 in pure C. The "p" in the name indicates "plus" or "progress".

The purpose of writing Mrtp was to refresh my knowledge of C++, raytracing 
and 3D computer graphics. Also to experiment with linking of a project to 
third-party libraries, for example png++.


## Installation instructions

For writing output files, Mrtp depends on the 
[png++ library](http://www.nongnu.org/pngpp/), a C++ wrapper for libpng. 
In the first step, run "make" in the "png++" folder. This will automatically 
download and unpack all necessary files. png++ is a header library and does 
not require compilation. 

Then, run "make" in the main folder. An executable file "mrtp" will be 
generated that is ready to run. Typing in "mrtp -h" will display a list of
available options and an example of use.


## Input file format

A scene (aka world) in Mrtp is generated based on an input file. The input 
file contains definitions of all "actors" in the scene, such as spheres, 
planes and cylinders. It also contains two non-actor definitions: 
a camera and a source of light.

The input file has the following structure:

```
# Example input.
camera
position  12.0    4.0    7.0
target     0.0    0.0    1.0
roll       0.0

light
position   5.0   -5.0    5.0

plane
center     0.0    0.0    0.0
normal     0.0    0.0    1.0
scale      0.15
texture   "textures/04univ2.png"

cylinder
center     0.0    0.0    0.0
direction  0.0    0.0    1.0
radius     1.0
span      -1.0
texture   "textures/qubodup-light_wood.png"

sphere
position   3.0    5.0    1.2
radius     1.2
axis       0.0    1.0    0.5
texture   "textures/02camino.png"

<more actors or EOF>
```

Each entry defining an actor (or camera, light) starts from a label,
such as "sphere", "plane", "camera", etc. The label is followed by 
one or more parameter lines. The entry always ends with 
a blank line.

Comments start from the "#" sign. Everything after this sign 
is ignored.

See the "example.inp" file for an example scene.
