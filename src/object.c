#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <object.h>
#include <direct.h>
/**
 * Computes the normal vector of a triangle based on
 * the first two vectors in clockwise order defining
 * its edges.
 */
v normal(v v1, v v2)
{
    p ep1;
    p ep2;
    p normP;
    v norm;
    ep1.x = v1.p1.x - v1.p2.x;
    ep1.y = v1.p1.y - v1.p2.y;
    ep1.z = v1.p1.z - v1.p2.z;
    ep2.x = v2.p2.x - v1.p1.x;
    ep2.y = v2.p2.y - v1.p1.y;
    ep2.z = v2.p2.z - v1.p1.z;
    normP.x = -(ep1.y * ep2.z - ep1.z * ep2.y);
    normP.y = -(ep1.z * ep2.x - ep1.x * ep2.z);
    normP.z = -(ep1.x * ep2.y - ep1.y * ep2.x);
    norm.p1.x = 0;
    norm.p1.y = 0;
    norm.p1.z = 0;
    norm.p2 = normP;
    return norm;
}

/**
 * Creates a tri from 3 points
 */
tri triangle(p p1, p p2, p p3)
{
    tri t;
    t.v1.p1 = p1;
    t.v1.p2 = p2;
    t.v2.p1 = p2;
    t.v2.p2 = p3;
    t.v3.p1 = p3;
    t.v3.p2 = p1;
    t.norm = normal(t.v1, t.v2);
    return t;
}

/**
 * Accepts the name of an object file from the `mesh` directory
 * and 'creates' the object in space about the origin as an array
 * of triangles with vertices and normal vectors.
 */
tri *create_object(char *object_name, unsigned int *triangle_count)
{
    // Get the filename from object_name and read
    // the open the mesh file with that name
    FILE *obj_file_ptr;
    char c;
    char *path = malloc((size_t)256);
    path = strcat(strcat(_getcwd(path, 256), "\\mesh\\"), object_name);
    // char path[] = "C:\\Users\\fireb\\src\\github\\personal\\Renderer\\mesh\\octahedron.obj";
    printf("path is %s\n", path);
    obj_file_ptr = fopen(path, "r");
    if (obj_file_ptr == NULL)
    {
        perror("obj_file_ptr was null");
    }
    // Count the number of triangles in the file
    unsigned int tri_count = 0;
    while ((c = (char)fgetc(obj_file_ptr)) != EOF)
    {
        if (c == 'f')
        {
            tri_count = tri_count + 1;
            while (c != '\n' && c != EOF)
            {
                c = (char)fgetc(obj_file_ptr);
            }
        }
        else
            while (c != '\n' && c != EOF)
            {
                c = (char)fgetc(obj_file_ptr);
            }
    }
    fclose(obj_file_ptr);

    // Allocate memory for the object given the # of
    // triangles, reopen the file and start parsing
    // the points and triangles present.
    tri *object = malloc((size_t)(sizeof(tri) * tri_count));
    int triIndex = 0;
    if (object == NULL)
    {
        perror("failed to malloc object");
    }
    p *points = malloc((size_t)(sizeof(v) * tri_count * 3));
    int pointIndex = 0;
    char def[6];
    char float_buffer[8];
    obj_file_ptr = fopen(path, "r");
    if (obj_file_ptr == NULL)
    {
        perror("obj_file_ptr was null");
    }
    while ((c = (char)fgetc(obj_file_ptr)) != EOF)
    {
        if (c == 'v')
        {
            c = (char)fgetc(obj_file_ptr);
            for (int i = 0; i < 8; i++)
            {
                c = (char)fgetc(obj_file_ptr);
                float_buffer[i] = c;
            }
            points[pointIndex].x = strtof(float_buffer, NULL);

            c = (char)fgetc(obj_file_ptr);
            for (int i = 0; i < 8; i++)
            {
                c = (char)fgetc(obj_file_ptr);
                float_buffer[i] = c;
            }
            points[pointIndex].y = strtof(float_buffer, NULL);

            c = (char)fgetc(obj_file_ptr);
            for (int i = 0; i < 8; i++)
            {
                c = (char)fgetc(obj_file_ptr);
                float_buffer[i] = c;
            }
            points[pointIndex].z = strtof(float_buffer, NULL);

            pointIndex = pointIndex + 1;
        }
        else if (c == 'f')
        {
            for (int i = 0; i < 6; i++)
            {
                c = (char)fgetc(obj_file_ptr);
                def[i] = c;
            }
            int point1 = def[1] - '0';
            int point2 = def[3] - '0';
            int point3 = def[5] - '0';

            object[triIndex] = triangle(points[point1 - 1], points[point2 - 1], points[point3 - 1]);

            triIndex = triIndex + 1;
        }
        else
        {
            while (c != '\n' && c != EOF)
            {
                c = (char)fgetc(obj_file_ptr);
            }
        }
    }
    fclose(obj_file_ptr);

    free(points);

    // Store the triangle count at the output parameter and return
    // a pointer to the object.
    *triangle_count = tri_count;
    return object;
}

void print_object(tri *object, unsigned int tri_count)
{
    printf("+----------------+ Object  Definition +------------------+\n");
    for (unsigned int i = 0; i < tri_count; i++)
    {
        printf("  tri # %d of %d=>\n", i + 1, tri_count);
        printf("    v1   => (%f, %f, %f) -> (%f, %f, %f)\n", (double)object[i].v1.p1.x, (double)object[i].v1.p1.y, (double)object[i].v1.p1.z, (double)object[i].v1.p2.x, (double)object[i].v1.p2.y, (double)object[i].v1.p2.z);
        printf("    v2   => (%f, %f, %f) -> (%f, %f, %f)\n", (double)object[i].v2.p1.x, (double)object[i].v2.p1.y, (double)object[i].v2.p1.z, (double)object[i].v2.p2.x, (double)object[i].v2.p2.y, (double)object[i].v2.p2.z);
        printf("    v3   => (%f, %f, %f) -> (%f, %f, %f)\n", (double)object[i].v3.p1.x, (double)object[i].v3.p1.y, (double)object[i].v3.p1.z, (double)object[i].v3.p2.x, (double)object[i].v3.p2.y, (double)object[i].v3.p2.z);
        printf("    norm => (%f, %f, %f) -> (%f, %f, %f)\n", (double)object[i].norm.p1.x, (double)object[i].norm.p1.y, (double)object[i].norm.p1.z, (double)object[i].norm.p2.x, (double)object[i].norm.p2.y, (double)object[i].norm.p2.z);
    }
    printf("+----------------+ End Obj Definition +------------------+\n");
}