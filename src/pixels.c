#include <object.h>
#include <raster.h>
#include <camera.h>

/* Writes the pixel data from the source matrix to the destination matrix */
void write_pixels(int *source, int *destination, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(destination + i * width + j) = *(source + i * width + j);
        }
    }
}

/* Fills the pixel matrix destination with dummy data, used
 * to initialize the first frame. */
void write_frame_zero(int *destination, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(destination + i * width + j) = 0x00000000;
        }
    }
}

/**
 * Computes the screen coordinates of each corner of an object
 * and draws them on the screen without connecting them.
 *
 * this function is currently unusable because it requires
 * objects defined in WinMain to be passed in as arguments
 * from a TIMER callback function.
 *
 * TODO: Figure out how to save objects like the camera
 * somewhere that they can be accessed by all necessary components,
 * without making a mess at the global level.
 */
void draw_obj_corners(
    tri *faces, int face_count,
    int *destination, int width, int height,
    camera *cam,
    float *canvas_width,
    float *canvas_height,
    int *window_width,
    int *window_height)
{
    // All pixels black
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(destination + i * width + j) = 0x00000000;
        }
    }
    // Draw corners
    pixel rastered_1;
    pixel rastered_2;
    pixel rastered_3;
    for (int i = 0; i < face_count; i++)
    {
        int rastered_1_visible = rasterize_point(
            faces[i].v1.p1,
            cam,
            canvas_width,
            canvas_height,
            window_width,
            window_height,
            &rastered_1);
        int rastered_2_visible = rasterize_point(
            faces[i].v2.p1,
            cam,
            canvas_width,
            canvas_height,
            window_width,
            window_height,
            &rastered_2);
        int rastered_3_visible = rasterize_point(
            faces[i].v3.p1,
            cam,
            canvas_width,
            canvas_height,
            window_width,
            window_height,
            &rastered_3);
        *(destination + rastered_1.x * width + rastered_1.y) = 0xFFFFFF00;
        *(destination + rastered_2.x * width + rastered_2.y) = 0xFFFFFF00;
        *(destination + rastered_3.x * width + rastered_3.y) = 0xFFFFFF00;
    }
}

/* Paints the next black pixel red from top left to bottom right,
 * filling the rows first. Just a testing function. */
void draw_next_pixel(int *destination, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (*(destination + i * width + j) == 0x00000000)
            {
                *(destination + i * width + j) = 0x00FF0000;
                return;
            }
        }
    }
}