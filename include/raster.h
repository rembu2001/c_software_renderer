/**
 * Represents coordinates for a point on a raster,
 * where (x, y) = (0, 0) is the top-left.
 */
typedef struct pixel
{
    int x;
    int y;
} pixel;

/**
 * Used for Canvas/Viewport coordinates and
 * normalized device coordinates. Represents
 * any arbitrary 2 dimensional point in
 * floating point.
 */
typedef struct fPoint
{
    float x;
    float y;
} fPoint;

/**
 * Sets the value *dest to the pixel coordinates corresponding
 * to the worldPoint p, and returns true (1) if the point will be
 * visible on screen, otherwise returns false (0).
 *
 * @param worldPoint the point in the world coordinate system
 * to be represented on screen
 * @param cam the camera viewing the worldPoint
 * @param canvasWidth width of the viewport
 * @param canvasHeight height of the viewport
 * @param screenWidth width of the raster
 * @param screenHeight height of the raster
 * @param dest location to store corresponding raster coordinates
 * of the worldPoint
 */
int rasterize_point();