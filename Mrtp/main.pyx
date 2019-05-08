#-------------------------------------------------------------------------------
# . File      : main.pyx
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
import exceptions, os.path


DEF DEFAULT_WIDTH = 640
DEF DEFAULT_HEIGHT = 480
DEF DEFAULT_FOV = 93.0
DEF DEFAULT_DISTANCE = 60.0
DEF DEFAULT_SHADOW = 0.25
DEF DEFAULT_BIAS = 0.001
DEF DEFAULT_DEPTH = 3
DEF DEFAULT_FILENAME = "scene.png"

DEF DEFAULT_REFLECT = 0.0
DEF DEFAULT_SCALE = 0.15

DEF MIN_WIDTH = (DEFAULT_WIDTH  //  2)
DEF MAX_WIDTH = (DEFAULT_WIDTH  *  10)
DEF MIN_HEIGHT = (DEFAULT_HEIGHT //  2)
DEF MAX_HEIGHT = (DEFAULT_HEIGHT *  10)
DEF MIN_DEPTH = 0
DEF MAX_DEPTH = 10
DEF MIN_FOV = 50.0
DEF MAX_FOV = 170.0

DEF DEFAULT_THREADS = 1
DEF MIN_THREADS = 0
DEF MAX_THREADS = 64


cdef class PyWorld:
    def __cinit__ (self, verbose=True):
        self.cObject = new World ()
        self.verbose = verbose

    def __dealloc__ (self):
        del self.cObject

    def _Text (self, message):
        if (self.verbose):
            print (" wrld> %s" % message)

    def AddCamera (self, PyCamera camera):
        self.cObject.add_camera (camera.cObject)

    def AddLight (self, PyLight light):
        self.cObject.add_light (light.cObject)

    def AddPlane (self, PyPlane plane):
        self.cObject.add_plane (plane.cObject)

    def AddSphere (self, PySphere sphere):
        self.cObject.add_sphere (sphere.cObject)

    def AddCylinder (self, PyCylinder cylinder):
        self.cObject.add_cylinder (cylinder.cObject)


#===============================================================================
cdef class PyRenderer:
    def __cinit__ (self, 
                   PyWorld world, 
                   int width=DEFAULT_WIDTH, 
                   int height=DEFAULT_HEIGHT, 
                   float fov=DEFAULT_FOV, 
                   float distance=DEFAULT_DISTANCE, 
                   float shadow=DEFAULT_SHADOW, 
                   float bias=DEFAULT_BIAS, 
                   int maxdepth=DEFAULT_DEPTH, 
                   int nthreads=DEFAULT_THREADS, 
                   verbose=True ):

        if (width < MIN_WIDTH) or (width > MAX_WIDTH) or (height < MIN_HEIGHT) or (height > MAX_HEIGHT):
            raise exceptions.StandardError ("Resolution is out of range.")

        if (fov < MIN_FOV) or (fov > MAX_FOV):
            raise exceptions.StandardError ("Field of vision is out of range.")

        if (nthreads < MIN_THREADS) or (nthreads > MAX_THREADS):
            raise exceptions.StandardError ("Number of threads is out of range.")

        if (maxdepth < MIN_DEPTH) or (maxdepth > MAX_DEPTH):
            raise exceptions.StandardError ("Number of reflective rays is out of range.")

        self.cObject = new Renderer (world.cObject, width, height, fov, distance, shadow, bias, maxdepth, nthreads)
        self.verbose = verbose

        self._Text ("w=%d h=%d fov=%.1f dist=%.1f shad=%.2f bias=%.3f dep=%d cpu=%d" % (width, height, fov, distance, shadow, bias, maxdepth, nthreads))

    def __dealloc__ (self):
        del self.cObject

    def _Text (self, message):
        if (self.verbose):
            print (" rndr> %s" % message)

    def Render (self):
        cdef float seconds;

        self._Text ("Rendering scene...")
        seconds = self.cObject.render_scene ()

        self._Text ("Done, elapsed time: %.2f sec" % seconds)

    def WriteScene (self, filename=DEFAULT_FILENAME):
        cdef char *cfilename = filename

        self.cObject.write_scene (cfilename)
        self._Text ("Wrote file %s" % filename)


#===============================================================================
cdef class PyCamera:
    def __cinit__ (self, center, target, float roll=0.0):
        self.x_ = center[0]
        self.y_ = center[1]
        self.z_ = center[2]
        self.tx_ = target[0]
        self.ty_ = target[1]
        self.tz_ = target[2]
        self.roll_ = roll
        self.cObject = new Camera (&self.x_, &self.tx_, &self.roll_)

    def __dealloc__ (self):
        del self.cObject

    def Center (self):
        return (self.x_, self.y_, self.z_)

    def SetCenter (self, xyz):
        self.x_ = xyz[0]
        self.y_ = xyz[1]
        self.z_ = xyz[2]

    def Target (self):
        return (self.tx_, self.ty_, self.tz_)

    def SetTarget (self, xyz):
        self.tx_ = xyz[0]
        self.ty_ = xyz[1]
        self.tz_ = xyz[2]

    def Roll (self):
        return self.roll_

    def SetRoll (self, float value):
        self.roll_ = value


cdef class PyLight:
    def __cinit__ (self, center):
        self.x_ = center[0]
        self.y_ = center[1]
        self.z_ = center[2]
        self.cObject = new Light (&self.x_)

    def __dealloc__ (self):
        del self.cObject

    def Center (self):
        return (self.x_, self.y_, self.z_)

    def SetCenter (self, xyz):
        self.x_ = xyz[0]
        self.y_ = xyz[1]
        self.z_ = xyz[2]


cdef class PyPlane:
    def __cinit__ (self, center, normal, texture, float scale=DEFAULT_SCALE, float reflect=DEFAULT_REFLECT):
        cdef float ccenter[3]
        cdef float cnormal[3]
        cdef char *ctexture

        if (not os.path.exists (texture)):
            raise exceptions.StandardError ("Error opening texture file %s" % texture)

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]

        cnormal[0] = normal[0]
        cnormal[1] = normal[1]
        cnormal[2] = normal[2]

        ctexture = texture
        self.cObject = new Plane (ccenter, cnormal, scale, reflect, ctexture)

    def __dealloc__ (self):
        del self.cObject


cdef class PySphere:
    def __cinit__ (self, center, texture, float radius=1.0, axis=(0.0, 0.0, 1.0), float reflect=DEFAULT_REFLECT):
        cdef float ccenter[3]
        cdef float caxis[3]
        cdef char *ctexture

        if (not os.path.exists (texture)):
            raise exceptions.StandardError ("Error opening texture file %s" % texture)

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]
        
        caxis[0] = axis[0]
        caxis[1] = axis[1]
        caxis[2] = axis[2]

        ctexture = texture
        self.cObject = new Sphere (ccenter, radius, caxis, reflect, ctexture)


cdef class PyCylinder:
    def __cinit__ (self, center, direction, texture, float radius=1.0, float span=-1.0, float reflect=DEFAULT_REFLECT):
        cdef float ccenter[3]
        cdef float cdirection[3]
        cdef char *ctexture

        if (not os.path.exists (texture)):
            raise exceptions.StandardError ("Error opening texture file %s" % texture)

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]
        
        cdirection[0] = direction[0]
        cdirection[1] = direction[1]
        cdirection[2] = direction[2]

        ctexture = texture
        self.cObject = new Cylinder (ccenter, cdirection, radius, span, reflect, ctexture)
