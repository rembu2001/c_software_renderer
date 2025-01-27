#include <library.h>
#include <math.h>
#include <stdio.h>

#define F_PI 3.14159f

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

/**
 * Creates a line in a raster given two pixels.
 */
void draw_raster_line(int *destination, pixel p1, pixel p2, int width, int height)
{
    float x1 = (float)p1.x;
    float y1 = (float)p1.y;
    float x2 = (float)p2.x;
    float y2 = (float)p2.y;
    float dx = (x2 - x1) / 2000.0f;
    float dy = (y2 - y1) / 2000.0f;
    while (x1 - x2 > 0.5f || x2 - x1 > 0.5f || y1 - y2 > 0.5f || y2 - y1 > 0.5f)
    {
        if ((int)x1 >= 0 && (int)x1 < width && (int)y1 >= 0 && (int)y1 < height)
        {
            *(destination + (int)y1 * width + (int)x1) = 0x00FF0000;
        }
        x1 += dx;
        y1 += dy;
    }
}

/**
 * Computes the screen coordinates of each corner of an object
 * and draws lines between them.
 */
void draw_obj_edges(
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
        rasterize_point(
            faces[i].v1.p1,
            cam,
            canvas_width,
            canvas_height,
            window_width,
            window_height,
            &rastered_1);
        rasterize_point(
            faces[i].v2.p1,
            cam,
            canvas_width,
            canvas_height,
            window_width,
            window_height,
            &rastered_2);
        rasterize_point(
            faces[i].v3.p1,
            cam,
            canvas_width,
            canvas_height,
            window_width,
            window_height,
            &rastered_3);

        draw_raster_line(
            destination,
            rastered_1, rastered_2,
            *window_width, *window_height);
        draw_raster_line(
            destination,
            rastered_2, rastered_3,
            *window_width, *window_height);
        draw_raster_line(
            destination,
            rastered_3, rastered_1,
            *window_width, *window_height);
    }
}

/**
 * Generates a color to fill a triangle with based on and
 * angle between a viewer and a faces normal vector.
 */
int color_from_angle(float angle)
{
    float normalized_color = 1.0f - (angle / F_PI);
    int full = (int)((float)0xFF * normalized_color);
    int color = (full << 16) + (full << 8) + full;
    return color;
}

/**
 * An unfinished function that colors the faces of the triangle.
 * Currently uses a naive algorithm that dtermines if the normal
 * vector of a given face presents the outward part of the triangle
 * to the camera, which doesn't work well when the object is close
 * to the camera.
 */
void draw_obj_faces(
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

    for (int i = 0; i < (int)face_count; i++)
    {
        /**
         * Determine if this face is pointed
         * toward the camera.
         */
        v viewing_vector = {
            {
                0,
                0,
                0,
            },
            {-(cam->coordinateSystem)[0][2], -(cam->coordinateSystem)[1][2], -(cam->coordinateSystem)[2][2]}};
        v norm = faces[i].norm;
        float dot_product =
            viewing_vector.p2.x * (norm.p2.x - norm.p1.x) +
            viewing_vector.p2.y * (norm.p2.y - norm.p1.y) +
            viewing_vector.p2.z * (norm.p2.z - norm.p1.z);
        float magnitude_view = sqrtf(
            viewing_vector.p2.x * viewing_vector.p2.x +
            viewing_vector.p2.y * viewing_vector.p2.y +
            viewing_vector.p2.z * viewing_vector.p2.z);
        float magnitude_norm = sqrtf(
            norm.p2.x * norm.p2.x +
            norm.p2.y * norm.p2.y +
            norm.p2.z * norm.p2.z);
        float angle = acosf((dot_product) / (magnitude_view * magnitude_norm));
        if (angle < 3 * (F_PI / 4.0f))
        {
            /**
             * Calculate the color and corners
             * of the triangle.
             */
            int color = color_from_angle(angle);
            pixel rastered_1;
            pixel rastered_2;
            pixel rastered_3;
            rasterize_point(
                faces[i].v1.p1,
                cam,
                canvas_width,
                canvas_height,
                window_width,
                window_height,
                &rastered_1);
            rasterize_point(
                faces[i].v2.p1,
                cam,
                canvas_width,
                canvas_height,
                window_width,
                window_height,
                &rastered_2);
            rasterize_point(
                faces[i].v3.p1,
                cam,
                canvas_width,
                canvas_height,
                window_width,
                window_height,
                &rastered_3);
            /**
             * Create a bounding box around the
             * triangle.
             */
            int small_x, small_y, big_x, big_y;
            if (rastered_1.x > rastered_2.x)
            {
                if (rastered_1.x > rastered_3.x)
                {
                    big_x = rastered_1.x;
                    if (rastered_2.x < rastered_3.x)
                    {
                        small_x = rastered_2.x;
                    }
                    else
                    {
                        small_x = rastered_3.x;
                    }
                }
                else
                {
                    small_x = rastered_2.x;
                    big_x = rastered_3.x;
                }
            }
            else if (rastered_2.x > rastered_3.x)
            {
                big_x = rastered_2.x;
                if (rastered_1.x < rastered_3.x)
                {
                    small_x = rastered_1.x;
                }
                else
                {
                    small_x = rastered_3.x;
                }
            }
            else
            {
                big_x = rastered_3.x;
                small_x = rastered_1.x;
            }
            if (rastered_1.y > rastered_2.y)
            {
                if (rastered_1.y > rastered_3.y)
                {
                    big_y = rastered_1.y;
                    if (rastered_2.y < rastered_3.y)
                    {
                        small_y = rastered_2.y;
                    }
                    else
                    {
                        small_y = rastered_3.y;
                    }
                }
                else
                {
                    small_y = rastered_2.y;
                    big_y = rastered_3.y;
                }
            }
            else if (rastered_2.y > rastered_3.y)
            {
                big_y = rastered_2.y;
                if (rastered_1.y < rastered_3.y)
                {
                    small_y = rastered_1.y;
                }
                else
                {
                    small_y = rastered_3.y;
                }
            }
            else
            {
                big_y = rastered_3.y;
                small_y = rastered_1.y;
            }
            big_x = big_x >= *window_width ? *window_width - 1 : big_x;
            big_y = big_y >= *window_height ? *window_height - 1 : big_y;
            small_x = small_x < 0 ? 0 : small_x;
            small_y = small_y < 0 ? 0 : small_y;
            /**
             * Fill in points in the traingle according
             * to the Pineda edge function.
             */
            for (int y = small_y; y <= big_y; y++)
            {
                for (int x = small_x; x <= big_x; x++)
                {
                    if (
                        (x - rastered_1.x) * (rastered_2.y - rastered_1.y) - (y - rastered_1.y) * (rastered_2.x - rastered_1.x) >= 0 &&
                        (x - rastered_2.x) * (rastered_3.y - rastered_2.y) - (y - rastered_2.y) * (rastered_3.x - rastered_2.x) >= 0 &&
                        (x - rastered_3.x) * (rastered_1.y - rastered_3.y) - (y - rastered_3.y) * (rastered_1.x - rastered_3.x) >= 0)
                    {
                        *(destination + y * width + x) = color;
                    }
                }
            }
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