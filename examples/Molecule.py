#-------------------------------------------------------------------------------
# . File      : Molecule.py
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
import sys, collections, math

sys.path.append ("..")
sys.path.append ("/home/mikolaj/github/MolarisTools")

from MolarisTools.Scripts import BondsFromDistances
from Mrtp import PyWorld, PyRenderer, PyCamera, PyLight, PyPlane, PySphere, PyCylinder


Atom = collections.namedtuple ("Atom", "label  serial  x  y  z")

radiiTextures = {
    "C"   :  ( 1.70 , "../textures/trak_light2b.png" ), 
    "H"   :  ( 1.00 , "../textures/trak_light2b.png" ), 
    "N"   :  ( 1.55 , "../textures/trak_light2b.png" ), 
    "O"   :  ( 1.50 , "../textures/trak_light2b.png" ), 
    "P"   :  ( 2.00 , "../textures/trak_light2b.png" ), 
    "S"   :  ( 1.80 , "../textures/trak_light2b.png" ), 
    "FE"  :  ( 1.30 , "../textures/trak_light2b.png" ), 
    "ZN"  :  ( 1.34 , "../textures/trak_light2b.png" ), 
    "MG"  :  ( 1.30 , "../textures/trak_light2b.png" ),  }


class Molecule:
    def __init__ (self, path="molecule.xyz"):
        self.path = path

    @property
    def natoms (self):
        if hasattr (self, "reference"):
            return len (self.reference)
        return 0

    def SetUp (self):
        lines = open (self.path).readlines ()[2:]
        self.reference = []
        
        for (i, line) in enumerate (lines, 1):
            (label, x, y, z) = line.split ()
            atom = Atom (label=label, serial=i, x=float (x), y=float (y), z=float (z))
            self.reference.append (atom)

        self.bonds = BondsFromDistances (self.reference)

    def ScaleMove (self, scale=2.0):
        (xt, yt, zt) = (0.0, 0.0, 0.0)
        for (symbol, serial, x, y, z) in self.reference:
            xt += x
            yt += y
            zt += z
        s = 1.0 / self.natoms
        (xo, yo, zo) = (xt*s, yt*s, zt*s)
        
        temporary = []
        for (i, atom) in enumerate (self.reference, 1):
            (label, serial, x, y, z) = atom
            new = Atom (label=label, serial=i, x=scale*(x-xo), y=scale*(y-yo), z=scale*(z-zo))
            temporary.append (new)

        self.reference = temporary

    def _GenerateSpheres (self, scale=0.35):
        spheres = []

        for (label, serial, x, y, z) in self.reference:
            (radius, texture) = radiiTextures[label]
            sphere = PySphere (center=(x, y, z), radius=(radius*scale), texture=texture)
            spheres.append (sphere)
        return spheres

    def _GenerateCylinders (self, radius=0.2, bondTexture="../textures/qubodup-light_wood.png"):
        cylinders = []

        for pair in self.bonds:
            ((i, seriala, labela), (j, serialb, labelb)) = pair
            (atoma, atomb) = (self.reference[i], self.reference[j])
        
            (cx, cy, cz) = ((atoma.x+atomb.x)/2, (atoma.y+atomb.y)/2, (atoma.z+atomb.z)/2)
            (dx, dy, dz) = (atoma.x-atomb.x, atoma.y-atomb.y, atoma.z-atomb.z)
            span = math.sqrt (dx**2 + dy**2 + dz**2) * 0.5
        
            cylinder = PyCylinder (center=(cx, cy, cz), direction=(dx, dy, dz), radius=radius, span=span, texture=bondTexture)
            cylinders.append (cylinder)
        return cylinders

    def FillWorld (self, world):
        spheres = self._GenerateSpheres ()

        for sphere in spheres:
            world.AddSphere (sphere)

        cylinders = self._GenerateCylinders ()

        for cylinder in cylinders:
            world.AddCylinder (cylinder)


#===============================================================================
camera = PyCamera (center=(-5.0, 10.0, 5.0), target=(0.0, 0.0, 0.0), roll=0.0)
light = PyLight (center=(-3.0, 3.0, 11.0))
floor = PyPlane (center=(0.0, 0.0, -10.0), normal=(0.0, 0.0, 1.0), scale=0.15, texture="../textures/01tizeta_floor_f.png")
wall = PyPlane (center=(0.0, -16.0, 0.0), normal=(0.0, 1.0, 0.0), scale=0.15, reflect=0.65, texture="../textures/trak2_tile1b.png")
wallb = PyPlane (center=(16.0, 0.0, 0.0), normal=(-1.0, 0.0, 0.0), scale=0.15, texture="../textures/02camino.png")

world = PyWorld ()
world.AddCamera (camera)
world.AddLight (light)
world.AddPlane (floor)
world.AddPlane (wall)
world.AddPlane (wallb)

molecule = Molecule (path="molecule.xyz")
molecule.SetUp ()
molecule.ScaleMove ()
molecule.FillWorld (world)

renderer = PyRenderer (world, width=640, height=480)
renderer.Render ()
renderer.WriteScene ("molecule.png")
