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