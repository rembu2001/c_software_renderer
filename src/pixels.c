void write_pixels(int *source, int *destination, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(destination + i * width + j) = *(source + i * width + j);
        }
    }
}

void write_frame_zero(int *destination, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(destination + i * width + j) = 0x00000000;
        }
    }
}

void draw_next_pixel(int *destination, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (*(destination + i * width + j) == 0x00000000)
            {
                *(destination + i * width + j) = 0x00FF0000;
                return;
            }
        }
    }
}