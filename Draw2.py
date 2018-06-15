#-------------------------------------------------------------------------------
# . File      : Draw2.py
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
from Mrtp import World, Renderer, Camera, Light, Plane, Sphere, Cylinder


camera = Camera (center=(10.0, -10.0, 3.0), target=(0.0, 0.0, 5.0), roll=0.0)
light = Light (center=(5.0, 5.0, 5.0))

pa = Plane (center=(0.0, 0.0, 0.0), normal=(0.0, 0.0, 1.0), scale=0.15, reflect=1.0, texture="textures/04univ3.png")
pb = Plane (center=(0.0, 0.0, 17.0), normal=(0.0, 0.0, -1.0), scale=0.15, reflect=1.0, texture="textures/trak2_tile1b.png")
pc = Plane (center=(-17.0, 0.0, 0.0), normal=(1.0, 0.0, 0.0), scale=0.25, reflect=1.0, texture="textures/trak_light2b.png")
pd = Plane (center=(0.0, -17.0, 0.0), normal=(0.0, 1.0, 0.0), scale=0.15, reflect=1.0, texture="textures/01tizeta_floor_g.png")
pe = Plane (center=(17.0, 0.0, 0.0), normal=(-1.0, 0.0, 0.0), scale=0.15, reflect=1.0, texture="textures/01tizeta_floor_f.png")
pf = Plane (center=(0.0, 17.0, 0.0), normal=(0.0, -1.0, 0.0), scale=0.15, reflect=1.0, texture="textures/04univ2.png")

ca = Cylinder (center=(5.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")
cb = Cylinder (center=(0.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")
cc = Cylinder (center=(-5.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")
cd = Cylinder (center=(0.0, 0.0, 15.0), direction=(1.0, 0.0, 0.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")


world = World ()

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

renderer = Renderer (world, width=1024, height=768)
renderer.Render ()
renderer.WriteScene ("test-scene-2.png")
