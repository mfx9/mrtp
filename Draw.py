#-------------------------------------------------------------------------------
# . File      : Draw.py
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
from Mrtp import World, Renderer


world = World ()

world.AddCamera (origin=(12.0, 4.0, 7.0), target=(0.0, 0.0, 1.0), roll=0.0)
world.AddLight (origin=(5.0, -5.0, 5.0))

world.AddPlane (center=(0.0, 0.0, 0.0), normal=(0.0, 0.0, 1.0), scale=0.15, reflect=1.0, texture="textures/04univ2.png")
world.AddPlane (center=(-12.0, 0.0, 0.0), normal=(1.0, 0.0, 0.0), scale=0.15, reflect=1.0, texture="textures/01tizeta_floor_g.png")
world.AddPlane (center=(0.0, -12.0, 0.0), normal=(0.0, 1.0, 0.0), scale=0.25, reflect=1.0, texture="textures/trak_light2b.png")

world.AddCylinder (center=(0.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=1.0, span=-1.0, reflect=1.0, texture="textures/qubodup-light_wood.png")

world.AddSphere (center=(3.0, 5.0, 1.2), radius=1.2, axis=(0.0, 1.0, 0.5), texture="textures/02camino.png")
world.AddSphere (center=(-3.0, 5.0, 1.2), radius=1.2, axis=(0.0, 1.0, 0.5), texture="textures/02camino.png")
world.AddSphere (center=(-1.0, -5.0, 1.2), radius=1.2, axis=(0.0, 1.0, 0.5), texture="textures/02camino.png")

renderer = Renderer (world, width=640, height=480)
renderer.Render ()
renderer.WriteScene ("test-scene.png")
