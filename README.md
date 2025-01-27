# Renderer

A software renderer written in C with no dependencies other than the standard library and Win32 API.

Currently renders meshes based on a custom object definition, but I'm hoping to add shading, anti-aliasing, and more of the rendering pipeline if I have extra time to come back to this project.

Run `make` to create `renderer.exe`.

## Implementation

### Object Representation

Objects are stored in a `.obj` inspired format, but are handled and parsed by hand (see `octahedron.obj` for an example). 

The format creates objects from a set of vertices labeled `v` and a set of faces constructed by those vertices labeled `f`. All other entries in object files are treated as comments. 

### Converting Scene to Raster

Points defined in a scene as part of an object are converted to pixels in a raster according to the perspective of a camera, represented by its transformation matrix and focal length, and fitted to the window size. This occurs via a 4 step pipeline seen in `raster.c`, where Scene/World Coordinates are transformed to the Camera's coordinate system, are mapped in perspective to a defined viewport, converted to normalized device coordinates, and finally raster space.

## References

+ [Wikipedia - Euler Angles](https://en.wikipedia.org/wiki/Euler_angles#Table_of_matrices) for coordinate system operations.
+ [Microsoft Learn: Winuser.h](https://learn.microsoft.com/en-us/windows/win32/api/winuser/), and [theForger's Win32 API Programming Tutorial](http://www.winprog.org/tutorial/simple_window.html) for interacting with Windows.
+ [Scratch a pixel](https://www.scratchapixel.com/index.html) for insight into, implementations of, background info on, and the details of Computer Graphics at a high level. 
+ The 1988 paper [Juan Pineda - A Parellel Algorithm for Polygon Rasterization](https://www.cs.drexel.edu/~deb39/Classes/Papers/comp175-06-pineda.pdf) for an algorithm to find points inside a triangle.