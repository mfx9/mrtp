#-------------------------------------------------------------------------------
# . File      : Scene.py
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
import sys
sys.path.append ("..")

from Mrtp import PyWorld, PyRenderer, PyCamera, PyLight, PyPlane, PySphere, PyCylinder


camera = PyCamera (center=(12.0, 4.0, 7.0), target=(0.0, 0.0, 1.0), roll=0.0)
light = PyLight (center=(5.0, -5.0, 5.0))

pa = PyPlane (center=(0.0, 0.0, 0.0), normal=(0.0, 0.0, 1.0), scale=0.15, texture="../textures/04univ2.png")
pb = PyPlane (center=(-12.0, 0.0, 0.0), normal=(1.0, 0.0, 0.0), scale=0.15, texture="../textures/01tizeta_floor_g.png")
pc = PyPlane (center=(0.0, -12.0, 0.0), normal=(0.0, 1.0, 0.0), scale=0.25, texture="../textures/trak_light2b.png")

ca = PyCylinder (center=(0.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=1.0, span=-1.0, texture="../textures/qubodup-light_wood.png")

sa = PySphere (center=(3.0, 5.0, 1.2), radius=1.2, axis=(0.0, 1.0, 0.5), texture="../textures/02camino.png")
sb = PySphere (center=(-3.0, 5.0, 1.2), radius=1.2, axis=(0.0, 1.0, 0.5), texture="../textures/02camino.png")
sc = PySphere (center=(-1.0, -5.0, 1.2), radius=1.2, axis=(0.0, 1.0, 0.5), texture="../textures/02camino.png")


world = PyWorld ()

world.AddCamera (camera)
world.AddLight (light)
world.AddPlane (pa)
world.AddPlane (pb)
world.AddPlane (pc)
world.AddCylinder (ca)
world.AddSphere (sa)
world.AddSphere (sb)
world.AddSphere (sc)

renderer = PyRenderer (world, width=1024, height=768)
renderer.Render ()
renderer.WriteScene ("test-scene.png")
