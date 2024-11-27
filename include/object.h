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

tri *create_object(char *object_name, unsigned int *triangle_count);

void print_object(tri *object, unsigned int tri_count);