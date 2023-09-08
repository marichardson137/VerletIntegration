# Verlet Integration (C)

![ezgif com-video-to-gif](https://github.com/marichardson137/VerletIntegration/assets/77594556/83056570-5de0-491c-aeaa-783b583da1d7)

### Synopsis
This is an implementation of [Verlet Integration](https://www.algorithm-archive.org/contents/verlet_integration/verlet_integration.html), a numerical method commonly used for approximating trajectories. It's made with the C programming language and [OpenGL](https://www.khronos.org/about/), a popular API for GPU rendering. 

### Key Features
- Simulate thousands of particles (at 60 FPS) experiencing real-time collisions, arbitrary forces, constraints, and linking.
- First person camera that can be controlled using the mouse and keyboard _or_ animated along a path ([Bézier curve](https://en.wikipedia.org/wiki/B%C3%A9zier_curve)).
- Custom [OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file) file parser to import models from [external applications](https://www.blender.org/) and convert mesh data into an OpenGL-friendly data structure.
- Substeps, mathematical approximations, and sampling for improved simulation _accuracy_ and _stability_.
- Algorithms to generate structures like recursive [Icospheres](https://en.wikipedia.org/wiki/Geodesic_polyhedron).

### Optimizations (WIP)
- [Instance rendering](https://learnopengl.com/Advanced-OpenGL/Instancing)
- Multithreading (deterministic vs. non-deterministic)
- [KD-Tree](https://en.wikipedia.org/wiki/K-d_tree) or [Octree](https://en.wikipedia.org/wiki/Octree) spatial partitioning

### References
1. Math module by [Felipe Ferreira da Silva](https://github.com/felselva/mathc/tree/master) for working with vectors, matrices, and common mathematical functions.
2. Implementation of KD-Tree spatial partitioning by [John Tsiombikas](https://github.com/jtsiomb/kdtree).
3. [GLEW](https://glew.sourceforge.net/) for "wrangling" OpenGL functions and [GLFW](https://www.glfw.org/) for window management.

### Applying/releasing an attractive force w/ the Keyboard
![ezgif com-video-to-gif (1)](https://github.com/marichardson137/VerletIntegration/assets/77594556/df4eabd7-b257-4cb7-a8c3-b81e706f7f51)

### Linking particles into a "Net"
![ezgif com-video-to-gif (2)](https://github.com/marichardson137/VerletIntegration/assets/77594556/9b7cde39-567f-459b-9474-305534c78618)

### Icosphere which maintains its pressure using [Hooke's Law](https://en.wikipedia.org/wiki/Hooke%27s_law)!
![ezgif com-video-to-gif (3)](https://github.com/marichardson137/VerletIntegration/assets/77594556/937feb63-ffb4-4247-838c-f48b08db6508)

### System Specs.
- MacBook Pro (13-inch, M1, 2020)
- Chip - Apple M1
- Memory - 8 GB
- OS - Monterey Version 12.1

I have no idea if this will run on your machine but you're welcome to try by calling `make` in the root directory followed by `./app`
