#-------------------------------------------------------------------------------
# . File      : Mrtp.pxd
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
cdef extern from "camera.hpp":
    cdef cppclass CCamera:
        CCamera (float *center, float *target, float *roll)

cdef extern from "light.hpp":
    cdef cppclass CLight:
        CLight (float *center)

cdef extern from "plane.hpp":
    cdef cppclass CPlane:
        CPlane (float *center, float *normal, float scale, float reflect, char *texture)

cdef extern from "sphere.hpp":
    cdef cppclass CSphere:
        CSphere (float *center, float radius, float *axis, float reflect, char *texture)

cdef extern from "cylinder.hpp":
    cdef cppclass CCylinder:
        CCylinder (float *center, float *direction, float radius, float span, float reflect, char *texture)

cdef class Camera:
    cdef float x_
    cdef float y_
    cdef float z_
    cdef float tx_
    cdef float ty_
    cdef float tz_
    cdef float roll_
    cdef CCamera *cObject

cdef class Light:
    cdef float x_
    cdef float y_
    cdef float z_
    cdef CLight *cObject

cdef class Plane:
    cdef CPlane *cObject

cdef class Sphere:
    cdef CSphere *cObject

cdef class Cylinder:
    cdef CCylinder *cObject


#===============================================================================
cdef extern from "world.hpp":

    cdef cppclass CWorld:
        CWorld () except +
        void AddCamera (CCamera *camera)
        void AddLight (CLight *light)
        void AddPlane (CPlane *plane)
        void AddSphere (CSphere *sphere)
        void AddCylinder (CCylinder *cylinder)

cdef class World:
    cdef CWorld *cObject
    cdef public object verbose 


#===============================================================================
cdef extern from "renderer.hpp":

    cdef cppclass CRenderer:
        CRenderer (CWorld *world, int width, int height, float fov, float distance, float shadowfactor, int maxdepth, int nthreads) except +
        float Render ()
        void WriteScene (char *filename)

cdef class Renderer:
    cdef CRenderer *cObject
    cdef public object verbose 
