#include <object.h>
#include <camera.h>
#include <stdlib.h>

/**
 * Returns a pointer to a camera facing the
 * negative z-axis positioned at the origin.
 */
camera *default_camera()
{
    /**
     * This hard-coded default World -> Camera matrix
     * is equal to it's inverse, so we can just use it
     * for both.
     */
    float mat[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, -2, 1}};

    camera *cam = malloc(sizeof(camera));

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cam->coordinateSystem[i][j] = mat[i][j];
            cam->inverse[i][j] = mat[i][j];
        }
    }

    return cam;
}