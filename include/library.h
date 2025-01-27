/**
 * Represents a point in 3d space
 */
typedef struct
{
    float x;
    float y;
    float z;
} p;

/**
 * Represents a vector between 2 points
 * in 3d space from p1 to p2
 */
typedef struct
{
    p p1;
    p p2;
} v;

/**
 * Represents a triangle in 3d space defined by
 * 3 vector edges and a normal vector
 */
typedef struct
{
    v v1;
    v v2;
    v v3;
    v norm;
} tri;

/**
 * Computes the normal vector of a triangle based on
 * the first two vectors in clockwise order defining
 * its edges.
 */
v normal(v v1, v v2);

/**
 * Accepts the name of an object file from the `mesh` directory
 * and 'creates' the object in space about the origin as an array
 * of triangles with vertices and normal vectors.
 */
tri *create_object(char *object_name, unsigned int *triangle_count);

/**
 * Outputs an object definition to stdout
 */
void print_object(tri *object, unsigned int tri_count);

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

/**
 * Returns a pointer to a camera facing the
 * negative z-axis positioned at the origin.
 */
camera *default_camera();

/* Writes the pixel data from the source matrix to the destination matrix */
void write_pixels(int *source, int *destination, int width, int height);

/* Fills the pixel matrix destination with dummy data, used
 * to initialize the first frame. */
void write_frame_zero(int *destination, int width, int height);

/* Paints the next black pixel red from top left to bottom right,
 * filling the rows first. */
void draw_next_pixel(int *destination, int width, int height);

/**
 * Draws the corners of a given object on screen.
 */
void draw_obj_corners(
    tri *faces, unsigned int face_count,
    int *destination,
    camera *cam,
    float *canvas_width,
    float *canvas_height,
    int *window_width,
    int *window_height);

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
    int *window_height);

/**
 * Performs a rotation on a camera according to the Euler angles
 * alpha, beta, and gamma (but does not alter the original
 * coordinate system, i.e. inv(inv(W->C))).
 * See https://en.wikipedia.org/wiki/Euler_angles#Table_of_matrices
 */
void rotate_camera(camera *cam, float alpha, float beta, float gamma);