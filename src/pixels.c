#include <object.h>
#include <raster.h>
#include <camera.h>

#include <stdio.h>

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
 */
void draw_obj_corners(
    tri *faces, unsigned int face_count,
    int *destination,
    camera *cam,
    float *canvas_width,
    float *canvas_height,
    int *window_width,
    int *window_height)
{
    // All pixels black
    int width = *window_width;
    int height = *window_height;
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
    for (int i = 0; i < (int)face_count; i++)
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

        if (rastered_1_visible)
        {
            *(destination + ((rastered_1.y * width) + rastered_1.x)) = 0x00FF0000;
        }
        if (rastered_2_visible)
        {
            *(destination + ((rastered_2.y * width) + rastered_2.x)) = 0x00FF0000;
        }
        if (rastered_3_visible)
        {
            *(destination + ((rastered_3.y * width) + rastered_3.x)) = 0x00FF0000;
        }
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