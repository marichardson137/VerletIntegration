# Verlet Integration (C)

### Synopsis
This is an implementation of [Verlet Integration](https://www.algorithm-archive.org/contents/verlet_integration/verlet_integration.html), a numerical method commonly used for approximating trajectories. It's made with the C programming language and [OpenGL](https://www.khronos.org/about/), a popular API for GPU rendering. 

### Key Features
- Simulate 2000+ particles at 60 FPS experiencing real-time collisions, arbitrary forces, constraints, and linking.
- 3D coordinate system, phong lighting, and a first person camera that can be controlled using the mouse and keyboard.
- Custom [OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file) file parser to import models from [external applications](https://www.blender.org/) and convert mesh data into an OpenGL friendly data structure. 
- Optimizations (WIP):
  - Instance rendering
  - Multithreading
  - KD-Tree spatial partitioning
  - Mathematical approximations

### References
1. [Math](https://github.com/felselva/mathc/blob/master/LICENSE) module by Felipe Ferreira da Silva for working with vectors, matrices, and common mathematical functions.
2. Implementation of [KD-Tree](https://github.com/jtsiomb/kdtree) spatial partitioning by John Tsiombikas.

### System Specs.
- MacBook Pro (13-inch, M1, 2020)
- Chip - Apple M1
- Memory - 8 GB
- OS - Monterey Version 12.1

I have no idea if this will run on your machine but you're welcome to try by calling `make` in the root directory followed by `./app`
