# Mrtp
<img src="./sample.png" alt="Sample image" width="400" />


A simple raytracing tool to explore combining C++ with Python.<br>

Released as open source software under the GNU GPL v3.0 license (s

Author: Mikolaj Feliks \<<mikolaj.feliks@gmail.com>\>ee COPYING).

## Installation instructions

For reading textures and writing output files, Mrtp depends on 
[png++](http://www.nongnu.org/pngpp/), a C++ wrapper for libpng. 

In step 1, run *make* in the png++/ folder. This will automatically 
download and unpack all necessary files. png++ is a header library and 
does not require compilation. Note, however, that you will need the 
libpng12-dev library installed as well.

In step 2, run *make* in the Mrtp/ folder. A Python library Mrtp.so 
will be generated that is ready to use in a Python script.

See the attached script Draw.py for an example of use.
