#-------------------------------------------------------------------------------
# . File      : main.pxd
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
cdef extern from "camera.hpp" namespace "mrtp":
    cdef cppclass Camera:
        Camera (float *center, float *target, float *roll)

cdef extern from "light.hpp" namespace "mrtp":
    cdef cppclass Light:
        Light (float *center)

cdef extern from "plane.hpp" namespace "mrtp":
    cdef cppclass Plane:
        Plane (float *center, float *normal, float scale, float reflect, char *texture)

cdef extern from "sphere.hpp" namespace "mrtp":
    cdef cppclass Sphere:
        Sphere (float *center, float radius, float *axis, float reflect, char *texture)

cdef extern from "cylinder.hpp" namespace "mrtp":
    cdef cppclass Cylinder:
        Cylinder (float *center, float *direction, float radius, float span, float reflect, char *texture)

cdef class PyCamera:
    cdef float x_
    cdef float y_
    cdef float z_
    cdef float tx_
    cdef float ty_
    cdef float tz_
    cdef float roll_
    cdef Camera *cObject

cdef class PyLight:
    cdef float x_
    cdef float y_
    cdef float z_
    cdef Light *cObject

cdef class PyPlane:
    cdef Plane *cObject

cdef class PySphere:
    cdef Sphere *cObject

cdef class PyCylinder:
    cdef Cylinder *cObject


#===============================================================================
cdef extern from "world.hpp" namespace "mrtp":

    cdef cppclass World:
        World () except +
        void add_camera (Camera *camera)
        void add_light (Light *light)
        void add_plane (Plane *plane)
        void add_sphere (Sphere *sphere)
        void add_cylinder (Cylinder *cylinder)

cdef class PyWorld:
    cdef World *cObject
    cdef public object verbose 


#===============================================================================
cdef extern from "renderer.hpp" namespace "mrtp":

    cdef cppclass Renderer:
        Renderer (World *world, int width, int height, float fov, float distance, float shadow, float bias, int maxdepth, int nthreads) except +
        float render_scene ()
        void write_scene (char *filename)

cdef class PyRenderer:
    cdef Renderer *cObject
    cdef public object verbose 
