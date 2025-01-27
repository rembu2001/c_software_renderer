/* Windows API Window Reference: http://www.winprog.org/tutorial/simple_window.html */

#include <windows.h>
#include <winuser.h>
#include <library.h>

#define INITIAL_WINDOW_WIDTH 640
#define INITIAL_WINDOW_HEIGHT 640
#define CANVAS_WIDTH 2.0f
#define CANVAS_HEIGHT 2.0f
#define REFRESH_RATE_MILLISECONDS 10

/* The timer that will indicate when to update the frame window */
const int ID_TIMER = 1;

/* Name of the window class to-be registered with the system. */
const char g_szClassName[] = "RendererWindowClass";

/* Global bitmap pointer that will always contain the most
 * recent frame data (most recently generated pixel matrix) */
HBITMAP global_bitmap = NULL;

/* Global array of pixels representing the current frame*/
int pixels[INITIAL_WINDOW_HEIGHT][INITIAL_WINDOW_WIDTH];

/* Global variables storing camera, scenes, and rendering info */
tri *rendered_obj_faces;
unsigned int rendered_obj_face_num;
camera *viewer_camera;
float *canvas_width;
float *canvas_height;
int *window_width;
int *window_height;

/* The Window Procedure: Handles incoming messages from the message loop (msg),
 * associated with the handle for the window (hwnd). */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    /* Switch statement for handling user input.
     * By default, sends the input to the Default Windows Procedure,
     * otherwise, explicitly handles the input (msg) */
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        /* On window destroy, destory the timer, window,
         * and BitMap resources */
        KillTimer(hwnd, ID_TIMER);
        DeleteObject(global_bitmap);
        PostQuitMessage(0);
        break;
    case WM_CREATE:
    {
        long long unsigned int timer_success;
        BITMAP bm;

        /* 1. Initialize pixel matrix*/
        write_frame_zero(&pixels[0][0], INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

        /* 2. Create BitMap resources */
        global_bitmap = CreateBitmap(
            INITIAL_WINDOW_WIDTH,
            INITIAL_WINDOW_HEIGHT,
            1,
            32,
            pixels);
        GetObject(global_bitmap, sizeof(bm), &bm);

        /* 3. Start the timer */
        timer_success = SetTimer(hwnd, ID_TIMER, REFRESH_RATE_MILLISECONDS, NULL);
        if (timer_success == 0)
        {
            MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
        }
        break;
    }
    case WM_PAINT:
    {
        /* Load the global bitmap */
        BITMAP bm;
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = SelectObject(hdcMem, global_bitmap);

        GetObject(global_bitmap, sizeof(bm), &bm);

        /* Paint the bitmap in the window */
        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_TIMER:
    {
        RECT rcClient;
        HDC hdc = GetDC(hwnd);

        BITMAP bm;
        PAINTSTRUCT ps;

        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = SelectObject(hdcMem, global_bitmap);

        GetClientRect(hwnd, &rcClient);

        /* Draw the object mesh */
        draw_obj_edges(
            rendered_obj_faces,
            rendered_obj_face_num,
            &pixels[0][0],
            viewer_camera,
            canvas_width,
            canvas_height,
            window_width,
            window_height);

        /* Rotate the camera */
        rotate_camera(viewer_camera, 0.01f, 0.01f, 0.003f);

        /* Update the global bitmap with the new frame data*/
        global_bitmap = CreateBitmap(
            INITIAL_WINDOW_WIDTH,
            INITIAL_WINDOW_HEIGHT,
            1,
            32,
            pixels);
        GetObject(global_bitmap, sizeof(bm), &bm);

        /* Load the bitmap to the window */
        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);

        ReleaseDC(hwnd, hdc);
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* Program entrance: registers the window, invokes it's creation, and starts the
 * window procedure and message loop. */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    /* Initialize the WINDCLASSEX Structure (wc / Window Class) */
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 1;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    /* Create the object in memory from a .obj file. */
    unsigned int *octahedron_face_count = malloc((size_t)sizeof(unsigned int));
    char obj_filename[] = "octahedron.obj";
    tri *octahedron = create_object(obj_filename, octahedron_face_count);

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

    /* Attempt to register the window class with the system */
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /* Create the window using the registered window class
     * Reference: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw */
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Renderer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        INITIAL_WINDOW_WIDTH,
        INITIAL_WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    /* Exit if the window handler being created was not initialized properly */
    if (hwnd == NULL)
    {
        MessageBox(
            NULL,
            "Window Creation Failed!",
            "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /* Show the window on the screen */
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* Windows API Window message loop that handles and returns
     * user input associated with the window. */
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}