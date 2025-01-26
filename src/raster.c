#include <object.h>
#include <math.h>
#include <camera.h>
#include <raster.h>
/*
 * "Points in a scene are defined in the world coordinate space.
 * To project them onto the surface of the canvas, however, we first
 * need to convert the 3D point coordinates from world space to camera
 * space. This conversion is accomplished by multiplying the point's
 * world coordinates by the inverse of the camera-to-world matrix.
 *
 * With the point now in camera space, we can accurately compute its
 * 2D coordinates on the canvas using the perspective projection
 * equations, specifically by dividing the point's coordinates by the
 * inverse of the point's z-coordinate."
 *
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points.html
 */

/**
 * Given a point in 3d space and a Camera, we can use a series of
 * transformations to map the point in 3d space to a point on the screen.
 *
 * Here, a 'point in 3d space' is a point with 3 coordinates, in the
 * general space called the "World Coordinate System", and corresponds
 * to a point in this absolute space.
 *
 * A Camera is effectively a coordinate system, which is represented by
 * a 4x4 transformation matrix applied to the World Coordinate System.
 * We can use the same matrix that defines the camera to transform points
 * from the World Coordinate System to the Camera Coordinate System.
 *
 * The transformation order is as follows:
 *
 * 1. World Coordinate System -> Camera Coordinate System
 *      - Taking a general point, we use the inverse of the 4x4
 *        transformation matrix that defines the camera to get
 *        the coordinate of the point in Camera Space, i.e. relative
 *        to the camera.
 *
 * 2. Camera Coordinate System -> Screen Space (Viewport/Canvas)
 *      - We use perspective projection to find the point on the
 *        image plane that represents the intersection of the
 *        viewing line from the camera position to the point,
 *        giving a 2d point defined in the image space.
 *
 * 3. Screen Space -> Normalized Device Coordinates
 *      - Normalized device coordinates are represented in the
 *        range [0, 1], so we convert to the point in screen
 *        space to NDC.
 *
 * 4. NDC -> Raster Space
 *      - This step is called the Viewport Transform, and involves
 *        deciding which pixels each point in NDC maps to, thereby
 *        converting a point in the range ([0, 1], [0, 1]) to a
 *        point on a 2d grid or raster, (pixels on the screen).
 */

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
int rasterize_point(
    p worldPoint,
    camera *cam,
    float *canvasWidth,
    float *canvasHeight,
    int *screenWidth,
    int *screenHeight,
    pixel *dest)
{

    // 1. World Coordinate System -> Camera Coordinate System
    p cameraPoint = {
        0, 0, 0};

    float a, b, c, w;

    a = worldPoint.x * cam->inverse[0][0] + worldPoint.y * cam->inverse[1][0] + worldPoint.z * cam->inverse[2][0] + cam->inverse[3][0];
    b = worldPoint.x * cam->inverse[0][1] + worldPoint.y * cam->inverse[1][1] + worldPoint.z * cam->inverse[2][1] + cam->inverse[3][1];
    c = worldPoint.x * cam->inverse[0][2] + worldPoint.y * cam->inverse[1][2] + worldPoint.z * cam->inverse[2][2] + cam->inverse[3][2];
    w = worldPoint.x * cam->inverse[0][3] + worldPoint.y * cam->inverse[1][3] + worldPoint.z * cam->inverse[2][3] + cam->inverse[3][3];

    cameraPoint.x = a / w;
    cameraPoint.y = b / w;
    cameraPoint.z = c / w;

    // 2. Camera Coordinate System -> Screen Space (Viewport/Canvas)
    fPoint canvasP = {
        cameraPoint.x / -cameraPoint.z,
        cameraPoint.y / -cameraPoint.z};

    /**
     * TODO: fix this, it is not identifying pixels outside the canvas
     * correctly so it tries to draw pixels that are off screen
     */

    // 3. Screen Space -> Normalized Device Coordinates
    fPoint ndcP = {
        (canvasP.x + *canvasWidth / 2) / *canvasWidth,
        (canvasP.y + *canvasHeight / 2) / *canvasHeight};

    // 4. NDC -> Raster Space
    dest->x = (int)(ndcP.x * (float)*screenWidth);
    dest->y = (int)(((float)1.0 - ndcP.y) * (float)*screenHeight);

    // If the pixel is outside the Canvas, point is not visible.
    if (canvasP.x > *canvasWidth || canvasP.x < -(*canvasWidth) || canvasP.y > *canvasHeight || canvasP.y < -(*canvasHeight))
    {
        return 0;
    }

    return 1;
};