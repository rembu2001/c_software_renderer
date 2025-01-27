#include <library.h>
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
        {0, 0, -3, 1}};

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
    float inv_copy[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[i][j] = (cam->coordinateSystem)[i][j];
            inv_copy[i][j] = (cam->inverse)[i][j];
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
     * Coordinate System = Rotation Matrix * Coordinate Sysetm
     * Invese = Inverse * (Rotation Matrix)^T
     */
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            (cam->coordinateSystem)[i][j] =
                R[i][0] * mat[0][j] +
                R[i][1] * mat[1][j] +
                R[i][2] * mat[2][j] +
                R[i][3] * mat[3][j];
            /**
             * The following makes assumptions, here consider
             * Coordinate System' = Rotation Matrix * Coordinate System
             * then
             * Inverse = inv(Coord.Sys) * inv(Rotation)
             * exlcusing translations, so we calculate the above
             * while ignoring the outer rows.
             */
            if (i != 3 && j != 3)
            {
                (cam->inverse)[i][j] =
                    R[j][0] * inv_copy[i][0] +
                    R[j][1] * inv_copy[i][1] +
                    R[j][2] * inv_copy[i][2] +
                    R[j][3] * inv_copy[i][3];
            }
        }
    }
}