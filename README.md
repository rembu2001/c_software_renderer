# Renderer

A software renderer written in C using the Win32 API

Run `make` to create `renderer.exe`.

## Implementation

### Object Representation

Objects are stored in a `.obj` inspired format, but are handled and parsed by hand (see `octahedron.obj` for an example). 

The format creates objects from a set of vertices labeled `v` and a set of faces constructed by those vertices labeled `f`. All other entries in object files are treated as comments. 

### Converting Scene to Raster

Points defined in a scene as part of an object are converted to pixels in a raster according to the perspective of a camera, represented by its transformation matrix and focal length, and fitted to the window size. This occurs via a 4 step pipeline seen in `raster.c`, where Scene/World Coordinates are transformed to the Camera's coordinate system, are mapped in perspective to a defined viewport, converted to normalized device coordinates, and finally raster space.

### TODO

Dependencies for transforming points from scene to raster need to be accessible from `WinMain` and `WndProc` so that they don't need to be passed around on every stack.

Need to implement remaining pieces of a wireframe rendering pipeline
- Decide if triangles are facing the screen
- Draw (and anti-alias) lines drawn in raster space based on their transformed coordinates
- Potentially store meta-data related to drawn triangles during the rendering pipeline so their luminence (and other attributes) can be determined.

## References

+ [Microsoft Learn: Winuser.h](https://learn.microsoft.com/en-us/windows/win32/api/winuser/), and [theForger's Win32 API Programming Tutorial](http://www.winprog.org/tutorial/simple_window.html) for interacting with Windows.
+ [Scratch a pixel](https://www.scratchapixel.com/index.html) for insight into, implementations of, background info on, and the details of Computer Graphics at a high level. 