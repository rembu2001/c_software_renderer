/**
 * A camera, as defined by it's transformation matrix,
 * where the coordinateSystem is the World->Camera
 * transformation matrix, and the inverse is the
 * Camera->World transformation matrix.
 */
typedef struct camera
{
    float coordinateSystem[4][4];
    float inverse[4][4];
} camera;

/**
 * Returns a pointer to a camera facing the
 * negative z-axis positioned at the origin.
 */
camera *default_camera();

/**
 * Performs a rotation on a camera according to the Euler angles
 * alpha, beta, and gamma (but does not alter the original
 * coordinate system, i.e. inv(inv(W->C))).
 * See https://en.wikipedia.org/wiki/Euler_angles#Table_of_matrices
 */
void rotate_camera(camera *cam, float alpha, float beta, float gamma);