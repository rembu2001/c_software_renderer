#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <library.h>

#define INITIAL_WINDOW_WIDTH 640
#define INITIAL_WINDOW_HEIGHT 640
#define CANVAS_WIDTH 2.0f
#define CANVAS_HEIGHT 2.0f
#define REFRESH_RATE_MILLISECONDS 10

/* Global bitmap pointer that will always contain the most
 * recent frame data (most recently generated pixel matrix) */
XImage* ximg;
GC gc;
Display* disp;
Window w;
int screen;

Atom WM_DELETE_WINDOW;

/* Global array of pixels representing the current frame*/
int pixels[INITIAL_WINDOW_HEIGHT][INITIAL_WINDOW_WIDTH];

/* Global variables storing camera, scenes, and rendering info */
tri *rendered_obj_faces;
unsigned int rendered_obj_face_num;
unsigned int* octahedron_face_count;
camera *viewer_camera;
float *canvas_width;
float *canvas_height;
int *window_width;
int *window_height;
tri* octahedron;

void callback(int signum);

/* The Window Procedure: Handles incoming messages from the message loop (msg),
 * associated with the handle for the window (hwnd). */
int event_loop(int scr)
{
    /* Switch statement for handling user input.
     * By default, sends the input to the Default Windows Procedure,
     * otherwise, explicitly handles the input (msg) */
    XEvent e;
    int running = 1;
    while (running) {
	XNextEvent(disp, &e);
	switch (e.type) {
	    case KeyPress:
	    	if (e.xkey.keycode == XKeysymToKeycode(disp, XStringToKeysym("q"))) {
		    running = 0;
		}
		break;
	    case ClientMessage:
		if (e.xclient.message_type == WM_DELETE_WINDOW) {
		    running = 0;
		}
		break;
	    default:
		break;
	}
    }
    free(canvas_height);
    free(canvas_width);
    free(window_height);
    free(window_width);
    free(octahedron);
    free(octahedron_face_count);
    XDestroyImage(ximg);
    XDestroyWindow(disp, w);
    XCloseDisplay(disp);
    return 0;
}

void callback(int signum) {
    draw_obj_edges(
	rendered_obj_faces,
	rendered_obj_face_num,
	&pixels[0][0],
	viewer_camera,
	canvas_width,
	canvas_height,
	window_width,
	window_height);

    rotate_camera(viewer_camera, 0.01f, 0.01f, 0.003f);

    if (!ximg) {
	ximg = XCreateImage(disp, DefaultVisual(disp, screen), (unsigned int) DefaultDepth(disp, screen),
			ZPixmap, 0, (char*) pixels, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, 32,
			INITIAL_WINDOW_WIDTH * 4);
    } else {
	memcpy(ximg->data, pixels, INITIAL_WINDOW_WIDTH * INITIAL_WINDOW_HEIGHT * sizeof(int));
    }

    XPutImage(disp, w, gc, ximg, 0, 0, 0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

    XFlush(disp);
}

/* Program entrance: registers the window, invokes it's creation, and starts the
 * window procedure and message loop. */
int main(int argc, char* argv[])
{
    disp = XOpenDisplay(NULL);
    if (!disp) {
	printf("Window initialisation failed.\n");
	return -1;
    }
    
    screen = DefaultScreen(disp);

    XSetWindowAttributes x_attributes;

    x_attributes.background_pixel = BlackPixel(disp, screen);
    x_attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | FocusChangeMask | ClientMessage;

    w = XCreateWindow(disp, RootWindow(disp, screen),
				XDisplayWidth(disp, screen) / 2,
				XDisplayHeight(disp, screen) / 2,
				(unsigned int) INITIAL_WINDOW_WIDTH,
				(unsigned int) INITIAL_WINDOW_HEIGHT, 1,
				CopyFromParent, InputOutput, CopyFromParent,
				CWBackPixel | CWEventMask,
				&x_attributes);
				
    XStoreName(disp, w, "C Software Renderer (Linux Edition)");
				
    WM_DELETE_WINDOW = XInternAtom(disp, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(disp, w, &WM_DELETE_WINDOW, 1);
    
    Cursor cursor = XCreateFontCursor(disp, XC_arrow);
    if (cursor) {
	XDefineCursor(disp, w, cursor);
    }

    XMapWindow(disp, w);
    gc = XCreateGC(disp, w, 0, NULL);
    
    write_frame_zero(&pixels[0][0], INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
		
    if (!ximg) {
        ximg = XCreateImage(disp, DefaultVisual(disp, screen), (unsigned int) DefaultDepth(disp, screen),
	    ZPixmap, 0, (char*) pixels, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT,
	    32, INITIAL_WINDOW_WIDTH * 4);
	if (!ximg) {
	    fprintf(stderr, "XCreateImage failed\n");
	    exit(1);
	}
    }

    struct itimerval timer;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = REFRESH_RATE_MILLISECONDS * 1000;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = REFRESH_RATE_MILLISECONDS * 1000;
    setitimer(ITIMER_REAL, &timer, NULL);

    signal(SIGALRM, callback);
    

    /* Create the object in memory from a .obj file. */
    octahedron_face_count = malloc((size_t)sizeof(unsigned int));
    char obj_filename[] = "octahedron.obj";
    octahedron = create_object(obj_filename, octahedron_face_count);

    /* Create the camera, define global scene parameters,
     * and pass object information up to global storage. */
    camera *cam = default_camera();
    window_width = (int *)malloc(sizeof(int));
    window_height = (int *)malloc(sizeof(int));
    *window_width = INITIAL_WINDOW_WIDTH;
    *window_height = INITIAL_WINDOW_HEIGHT;
    rendered_obj_faces = octahedron;
    rendered_obj_face_num = *octahedron_face_count;
    viewer_camera = cam;
    canvas_width = (float *)malloc(sizeof(float));
    canvas_height = (float *)malloc(sizeof(float));
    *canvas_width = CANVAS_WIDTH;
    *canvas_height = CANVAS_HEIGHT;

    int exit_status = event_loop(screen);

    return exit_status;
}
