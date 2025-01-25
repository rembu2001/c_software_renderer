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