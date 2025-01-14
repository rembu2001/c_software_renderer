/* Writes the pixel data from the source matrix to the destination matrix */
void write_pixels(int *source, int *destination, int width, int height);

/* Fills the pixel matrix destination with dummy data, used
 * to initialize the first frame. */
void write_frame_zero(int *destination, int width, int height);

/* Paints the next black pixel red from top left to bottom right,
 * filling the rows first. */
void draw_next_pixel(int *destination, int width, int height);