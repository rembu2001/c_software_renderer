# Renderer

A software renderer written in C using the Win32 API

Run `make` to create `renderer.exe`.

## Implementation

### Object Representation

Objects are stored in a `.obj` inspired format, but are handled and parsed by hand (see `octahedron.obj` for an example). 

The format creates objects from a set of vertices labeled `v` and a set of faces constructed by those vertices labeled `f`. All other entries in object files are treated as comments. 

### Planned ->

Create frames with pixel matrices display-agnostically

Display a bitmap on the screen that gets updated based on a timer for every frame, and loads data from a pixel matrix.

When the timer elapses, load the previously created pixel matrix to the bitmap, and start creating the next one.

Creating the next one:
    Create environment + camera data to render from -> Render data to pixel matrix

## References

+ [Microsoft Learn: Winuser.h](https://learn.microsoft.com/en-us/windows/win32/api/winuser/), and [theForger's Win32 API Programming Tutorial](http://www.winprog.org/tutorial/simple_window.html) for interacting with Windows.
+ [Scratch a pixel](https://www.scratchapixel.com/index.html) for insight into, implementations of, background info on, and the details of Computer Graphics at a high level. 