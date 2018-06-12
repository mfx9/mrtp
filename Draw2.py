#-------------------------------------------------------------------------------
# . File      : Draw2.py
# . Program   : mrtp
# . Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
# . License   : LGPL v3            (http://www.gnu.org/licenses/gpl-3.0.en.html)
#-------------------------------------------------------------------------------
from Mrtp import World, Renderer


world = World ()

world.AddCamera (origin=(10.0, -10.0, 3.0), target=(0.0, 0.0, 5.0), roll=0.0)
world.AddLight (origin=(5.0, 5.0, 5.0))

world.AddPlane (center=(0.0, 0.0, 0.0), normal=(0.0, 0.0, 1.0), scale=0.15, reflect=1.0, texture="textures/04univ3.png")
world.AddPlane (center=(0.0, 0.0, 17.0), normal=(0.0, 0.0, -1.0), scale=0.15, reflect=1.0, texture="textures/trak2_tile1b.png")
world.AddPlane (center=(-17.0, 0.0, 0.0), normal=(1.0, 0.0, 0.0), scale=0.25, reflect=1.0, texture="textures/trak_light2b.png")
world.AddPlane (center=(0.0, -17.0, 0.0), normal=(0.0, 1.0, 0.0), scale=0.15, reflect=1.0, texture="textures/01tizeta_floor_g.png")
world.AddPlane (center=(17.0, 0.0, 0.0), normal=(-1.0, 0.0, 0.0), scale=0.15, reflect=1.0, texture="textures/01tizeta_floor_f.png")
world.AddPlane (center=(0.0, 17.0, 0.0), normal=(0.0, -1.0, 0.0), scale=0.15, reflect=1.0, texture="textures/04univ2.png")

world.AddCylinder (center=(5.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")
world.AddCylinder (center=(0.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")
world.AddCylinder (center=(-5.0, 0.0, 0.0), direction=(0.0, 0.0, 1.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")
world.AddCylinder (center=(0.0, 0.0, 15.0), direction=(1.0, 0.0, 0.0), radius=0.7, span=-1.0, reflect=0.0, texture="textures/qubodup-light_wood.png")

renderer = Renderer (world, width=1024, height=768)
renderer.Render ()
renderer.WriteScene ("test-scene-2.png")
