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