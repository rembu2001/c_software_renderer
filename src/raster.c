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

// TODO: Implement this ... see the link above for some examples