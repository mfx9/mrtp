#-------------------------------------------------------------------------------
# . File      : Mrtp.pyx
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
import exceptions

DEF DEFAULT_WIDTH    =  640
DEF DEFAULT_HEIGHT   =  480
DEF DEFAULT_FOV      =   93.0
DEF DEFAULT_DISTANCE =   60.0
DEF DEFAULT_SHADOW   =    0.25
DEF DEFAULT_REFLECT  =    3
DEF DEFAULT_FILENAME =  "scene.png"

DEF MIN_WIDTH   =  (DEFAULT_WIDTH  //  2)
DEF MAX_WIDTH   =  (DEFAULT_WIDTH  *  10)
DEF MIN_HEIGHT  =  (DEFAULT_HEIGHT //  2)
DEF MAX_HEIGHT  =  (DEFAULT_HEIGHT *  10)
DEF MIN_REFLECT =      0
DEF MAX_REFLECT =     10
DEF MIN_FOV     =     50.0
DEF MAX_FOV     =    170.0

DEF DEFAULT_THREADS =  1
DEF MIN_THREADS     =  0
DEF MAX_THREADS     = 64


cdef class World:
    def __cinit__ (self):
        self.cObject = new CWorld ()

    def __dealloc__ (self):
        del self.cObject

    def _Text (self, message):
        print (" wrld> %s" % message)

    def AddCamera (self, Camera camera):
        self.cObject.AddCamera (camera.cObject)

    def AddLight (self, Light light):
        self.cObject.AddLight (light.cObject)

    def AddPlane (self, Plane plane):
        self.cObject.AddPlane (plane.cObject)

    def AddSphere (self, Sphere sphere):
        self.cObject.AddSphere (sphere.cObject)

    def AddCylinder (self, Cylinder cylinder):
        self.cObject.AddCylinder (cylinder.cObject)


#===============================================================================
cdef class Renderer:
    def __cinit__ (self, 
                   World world, 
                   int width=DEFAULT_WIDTH, 
                   int height=DEFAULT_HEIGHT, 
                   float fov=DEFAULT_FOV, 
                   float distance=DEFAULT_DISTANCE, 
                   float shadowfactor=DEFAULT_SHADOW, 
                   int maxdepth=DEFAULT_REFLECT, 
                   int reflshadow=0, 
                   int nthreads=DEFAULT_THREADS ):
        """Creates a renderer. 

        Keyword arguments:
            width         window width
            heigth        window heigth
            fov           field of vision, in degrees (default is 93)
            distance      distance to quench light (default is 60)
            shadowfactor  shadow factor (default is 0.25)
            maxdepth      recursion depth for reflected rays (default is 3)
            reflshadow    do (1) or do not (0) reflect rays from shadowed surfaces
            nthreads      rendering threads: 0 (auto), 1 (default), 2, 4, etc.
        """
        if ((width < MIN_WIDTH) or (width > MAX_WIDTH) or (height < MIN_HEIGHT) or (height > MAX_HEIGHT)):
            raise exceptions.StandardError ("Resolution is out of range.")

        if ((fov < MIN_FOV) or (fov > MAX_FOV)):
            raise exceptions.StandardError ("Field of vision is out of range.")

        if ((nthreads < MIN_THREADS) or (nthreads > MAX_THREADS)):
            raise exceptions.StandardError ("Number of threads is out of range.")

        if ((maxdepth < MIN_REFLECT) or (maxdepth > MAX_REFLECT)):
            raise exceptions.StandardError ("Number of reflective rays is out of range.")

        self.cObject = new CRenderer (world.cObject, 
                                      width, 
                                      height, 
                                      fov, 
                                      distance, 
                                      shadowfactor, 
                                      maxdepth, 
                                      reflshadow, 
                                      nthreads )

    def __dealloc__ (self):
        del self.cObject

    def _Text (self, message):
        print (" rndr> %s" % message)

    def Render (self):
        """Renders a scene."""
        cdef float seconds;

        self._Text ("Rendering scene...")
        seconds = self.cObject.Render ()

        self._Text ("Done. Elapsed time: %.2f sec" % seconds)

    def WriteScene (self, filename=DEFAULT_FILENAME):
        """Write a scene to a PNG file."""
        cdef char *cfilename = filename

        self.cObject.WriteScene (cfilename)
        self._Text ("Wrote file %s" % filename)


#===============================================================================
cdef class Camera:
    def __cinit__ (self, origin, target, float roll=0.0):
        cdef float corigin[3]
        cdef float ctarget[3]

        corigin[0] = origin[0]
        corigin[1] = origin[1]
        corigin[2] = origin[2]

        ctarget[0] = target[0]
        ctarget[1] = target[1]
        ctarget[2] = target[2]

        self.cObject = new CCamera (corigin, ctarget, roll)

    def __dealloc__ (self):
        del self.cObject


cdef class Light:
    def __cinit__ (self, origin):
        cdef float corigin[3]

        corigin[0] = origin[0]
        corigin[1] = origin[1]
        corigin[2] = origin[2]

        self.cObject = new CLight (corigin)

    def __dealloc__ (self):
        del self.cObject


cdef class Plane:
    def __cinit__ (self, center, normal, texture, float scale=0.15, float reflect=0.0):
        cdef float ccenter[3]
        cdef float cnormal[3]
        cdef char *ctexture

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]

        cnormal[0] = normal[0]
        cnormal[1] = normal[1]
        cnormal[2] = normal[2]

        ctexture = texture
        self.cObject = new CPlane (ccenter, cnormal, scale, reflect, ctexture)

    def __dealloc__ (self):
        del self.cObject


cdef class Sphere:
    def __cinit__ (self, center, texture, float radius=1.0, axis=(0.0, 0.0, 1.0), float reflect=0.0):
        cdef float ccenter[3]
        cdef float caxis[3]
        cdef char *ctexture

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]
        
        caxis[0] = axis[0]
        caxis[1] = axis[1]
        caxis[2] = axis[2]

        ctexture = texture
        self.cObject = new CSphere (ccenter, radius, caxis, reflect, ctexture)


cdef class Cylinder:
    def __cinit__ (self, center, direction, texture, float radius=1.0, float span=-1.0, float reflect=0.0):
        cdef float ccenter[3]
        cdef float cdirection[3]
        cdef char *ctexture

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]
        
        cdirection[0] = direction[0]
        cdirection[1] = direction[1]
        cdirection[2] = direction[2]

        ctexture = texture
        self.cObject = new CCylinder (ccenter, cdirection, radius, span, reflect, ctexture)
