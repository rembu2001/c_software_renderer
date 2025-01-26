#include <object.h>
#include <camera.h>
#include <stdlib.h>
#include <math.h>

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

/**
 * A hard coded camera which looks slightly down and to the side
 */
camera *angled_camera()
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

/**
 * Performs a rotation on a camera according to the Euler angles
 * alpha, beta, and gamma (but does not alter the original
 * coordinate system, i.e. inv(inv(W->C))).
 * See https://en.wikipedia.org/wiki/Euler_angles#Table_of_matrices
 */
void rotate_camera(camera *cam, float alpha, float beta, float gamma)
{
    float mat[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[i][j] = (cam->inverse)[i][j];
        }
    }

    /**
     * Compute the Rotation Matrix  R = X(a)Y(b)Z(g)
     */
    float s_a = sinf(alpha);
    float s_b = sinf(beta);
    float s_g = sinf(gamma);
    float c_a = cosf(alpha);
    float c_b = cosf(beta);
    float c_g = cosf(gamma);

    float R[4][4] = {{c_b * c_g, -1 * c_b * s_g, s_b, 0},
                     {c_a * s_g + c_g * s_a * s_b, c_a * c_g - s_a * s_b * s_g, -1 * c_b * s_a, 0},
                     {s_a * s_g - c_a * c_g * s_b, c_g * s_a + c_a * s_b * s_g, c_a * c_b, 0},
                     {0, 0, 0, 1}};

    /**
     * c_inv = R * c_inv
     */
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            (cam->inverse)[i][j] =
                R[i][0] * mat[0][j] +
                R[i][1] * mat[1][j] +
                R[i][2] * mat[2][j] +
                R[i][3] * mat[3][j];
        }
    }
}