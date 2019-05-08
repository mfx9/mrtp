#-------------------------------------------------------------------------------
# . File      : Scene2.py
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
import sys
sys.path.append ("..")

from Mrtp import PyWorld, PyRenderer, PyCamera, PyLight, PyPlane, PySphere, PyCylinder


camera = PyCamera (center=(10.0, -10.0, 3.0), target=(0.0, 0.0, 5.0), roll=0.0)
light = PyLight (center=(5.0, 5.0, 5.0))

pa = PyPlane (center=(0.0, 0.0, 0.0), normal=(0.0, 0.0, 1.0), scale=0.15, reflect=0.75, texture="../textures/trak2_tile1b.png")
pb = PyPlane (center=(0.0, 0.0, 17.0), normal=(0.0, 0.0, -1.0), scale=0.15, texture="../textures/04univ3.png")
pc = PyPlane (center=(-17.0, 0.0, 0.0), normal=(1.0, 0.0, 0.0), scale=0.25, texture="../textures/trak_light2b.png")
pd = PyPlane (center=(0.0, -17.0, 0.0), normal=(0.0, 1.0, 0.0), scale=0.15, texture="../textures/01tizeta_floor_g.png")
pe = PyPlane (center=(17.0, 0.0, 0.0), normal=(-1.0, 0.0, 0.0), scale=0.15, texture="../textures/01tizeta_floor_f.png")
pf = PyPlane (center=(0.0, 17.0, 0.0), normal=(0.0, -1.0, 0.0), scale=0.15, texture="../textures/04univ2.png")

ca = PyCylinder (center=(5.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, texture="../textures/qubodup-light_wood.png")
cb = PyCylinder (center=(0.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, texture="../textures/qubodup-light_wood.png")
cc = PyCylinder (center=(-5.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, texture="../textures/qubodup-light_wood.png")
cd = PyCylinder (center=(0.0, 0.0, 15.0), direction=(1.0, 0.0, 0.0), radius=0.7, span=-1.0, texture="../textures/qubodup-light_wood.png")


world = PyWorld ()

world.AddCamera (camera)
world.AddLight (light)
world.AddPlane (pa)
world.AddPlane (pb)
world.AddPlane (pc)
world.AddPlane (pd)
world.AddPlane (pe)
world.AddPlane (pf)
world.AddCylinder (ca)
world.AddCylinder (cb)
world.AddCylinder (cc)
world.AddCylinder (cd)

renderer = PyRenderer (world, width=1024, height=768)
renderer.Render ()
renderer.WriteScene ("test-scene-2.png")
