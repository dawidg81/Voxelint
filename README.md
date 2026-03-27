# Voxelint

Voxelint is another clone/inspiration of Minecraft Classic/ClassiCube game - an
interactive voxel game. It uses it's own 3D rasterizer, display based on SDL so
far.

# Code

Voxelint uses its own 3D engine and rasterizer. In `math.h` file all basic
calculations for 3d rendering is implemented. Voxelint uses SDL library for
display in window. It may use more libraries for display and pixel rendering in
the future.

## Renderer

Renderer is written there to draw such simple things as lines and triangles.
Triangles are the easiest geometry objects for computer to draw. Then from these
triangles we can create cubes. These cubes will be used for our Voxelint world.
Basically in `renderer.hpp` and `renderer.cpp` we implement drawing the whole
world.

### What can be optimized

In the first builds and versions our world is highly unoptimized. One of
important optimizations is already implemented, which is culling faces that
don't face air blocks. Optimizations are very important in 3D rendering, even
when we are rendering on GPU. 3D rendering and rasterizing is in general heavy
for computers.

No optimizations at all could be worse, since we would render all world blocks
which would require the game to calculate approximately 128x32x128 blocks if our
world is made of flat grass filling the bottom half of the map.

#### Greedy Meshing

The next very important optimization for rendering ahead is Greedy Meshing
technique/algorithm. It merges big square faces of cubes into one, so we have
less vertices to calculate.
