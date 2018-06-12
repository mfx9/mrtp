#-------------------------------------------------------------------------------
# . File      : Mrtp.pyx
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
cdef class World:
    def __cinit__ (self):
        self.cObject = new CWorld ()

    def __dealloc__ (self):
        del self.cObject

    def _Text (self, message):
        print (" wrld> %s" % message)

    def AddCamera (self, origin=(5.0, 5.0, 5.0), target=(0.0, 0.0, 0.0), float roll=0.0):
        cdef float corigin[3]
        cdef float ctarget[3]

        corigin[0] = origin[0]
        corigin[1] = origin[1]
        corigin[2] = origin[2]

        ctarget[0] = target[0]
        ctarget[1] = target[1]
        ctarget[2] = target[2]

        self.cObject.AddCamera (corigin, ctarget, roll)
        self._Text ("Added camera")

    def AddLight (self, origin=(0.0, 0.0, 0.0)):
        cdef float corigin[3]

        corigin[0] = origin[0]
        corigin[1] = origin[1]
        corigin[2] = origin[2]

        self.cObject.AddLight (corigin)
        self._Text ("Added light")

    def AddPlane (self, center=(0.0, 0.0, 0.0), normal=(0.0, 0.0, 1.0), float scale=0.15, float reflect=0.0, texture=None):
        cdef float ccenter[3]
        cdef float cnormal[3]
        cdef char *ctexture

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]

        cnormal[0] = normal[0]
        cnormal[1] = normal[1]
        cnormal[2] = normal[2]

        ctexture = texture if (texture) else ""
        self.cObject.AddPlane (ccenter, cnormal, scale, reflect, ctexture)
        self._Text ("Added plane")

    def AddCylinder (self, center=(0.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), float radius=1.0, float span=-1.0, float reflect=0.0, texture=None):
        cdef float ccenter[3]
        cdef float cdirection[3]
        cdef char *ctexture

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]

        cdirection[0] = direction[0]
        cdirection[1] = direction[1]
        cdirection[2] = direction[2]

        ctexture = texture if (texture) else ""
        self.cObject.AddCylinder (ccenter, cdirection, radius, span, reflect, ctexture)
        self._Text ("Added cylinder")

    def AddSphere (self, center=(0.0, 0.0, 0.0), float radius=1.0, axis=(0.0, 0.0, 1.0), float reflect=0.0, texture=None):
        cdef float ccenter[3]
        cdef float caxis[3]
        cdef char *ctexture

        ccenter[0] = center[0]
        ccenter[1] = center[1]
        ccenter[2] = center[2]

        caxis[0] = axis[0]
        caxis[1] = axis[1]
        caxis[2] = axis[2]

        ctexture = texture if (texture) else ""
        self.cObject.AddSphere (ccenter, radius, caxis, reflect, ctexture)
        self._Text ("Added sphere")


#===============================================================================
cdef class Renderer:
    def __cinit__ (self, 
                   World world, 
                   int width=640, 
                   int height=480, 
                   float fov=93.0, 
                   float distance=60.0, 
                   float shadowfactor=0.25, 
                   int maxdepth=5, 
                   int reflshadow=0, 
                   int nthreads=1 ):
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
        self._Text ("Rendering scene...")
        self.cObject.Render ()

        self._Text ("Done")

    def WriteScene (self, filename="scene.png"):
        cdef char *cfilename = filename

        self.cObject.WriteScene (cfilename)
        self._Text ("Wrote file %s" % filename)
