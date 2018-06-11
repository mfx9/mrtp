#-------------------------------------------------------------------------------
# . File      : Mrtp.pxd
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
cdef extern from "world.hpp":

    cdef cppclass CWorld:
        CWorld () except +
        void AddCamera (float *origin, float *target, float roll)
        void AddLight (float *origin)
        void AddPlane (float *center, float *normal, float scale, float reflect, char *path)
        void AddSphere (float *center, float *axis, float radius, float reflect, char *path)
        void AddCylinder (float *center, float *direction, float radius, float span, float reflect, char *path)

cdef class World:
    cdef CWorld *cObject


#===============================================================================
cdef extern from "renderer.hpp":

    cdef cppclass CRenderer:
        CRenderer (CWorld *world, int width, int height, float fov, float distance, float shadowfactor, int maxdepth, int reflshadow, int nthreads) except +
        void Render ()
        void WriteScene (char *filename)

cdef class Renderer:
    cdef CRenderer *cObject
