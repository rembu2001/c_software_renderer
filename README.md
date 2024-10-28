# Renderer

A software renderer written in C using the Win32 API

Run `make` to create `renderer.exe`.

## Implementation

### Planned ->

Create frames with pixel matrices display-agnostically

Display a bitmap on the screen that gets updated based on a timer for every frame, and loads data from a pixel matrix.

When the timer elapses, load the previously created pixel matrix to the bitmap, and start creating the next one.

Creating the next one:
    Create environment + camera data to render from -> Render data to pixel matrix

## References

+ [Microsoft Learn: Winuser.h](https://learn.microsoft.com/en-us/windows/win32/api/winuser/), and [theForger's Win32 API Programming Tutorial](http://www.winprog.org/tutorial/simple_window.html)  for interacting with Windows.