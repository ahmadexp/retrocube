#ifndef OBJECTS_H
#define OBJECTS_H 

#include "vector.h"
#include <stdbool.h> // true/false
#include <math.h> // round
#include <stddef.h> // size_t

enum {
    OBJ_CUBE=0,
    OBJ_RHOMBUS
};

typedef char color_t;

typedef struct shape {
    vec3i_t** vertices;
    vec3i_t** vertices_backup;
    vec3i_t* center;
    // number of vertices
    size_t n_vertices;
    // an array of characters defining the color of each face
    color_t colors[16];
    // type of shape to render, e.g. cube or rhumb
    int type;
} shape_t;

typedef struct ray {
    // origin is the centre of perspective in pinhole camera model
    // for now it is always set to (0, 0, 0)
    vec3i_t* orig;
    vec3i_t* end;
} ray_t;

/* 
 * plane in 3D assuming its equation is:
 * n_x*x + n_y*y + n_z*z + d = 0 (1)
 * or n.X + d = 0                (2)
 * , where n = (n_x, n_y, n_z) is the normal
 * , X = (x, y, y) a point on the plane
 * and d is the offset from the origin
 */
typedef struct plane {
    // d from eq. (2)
    int offset;
    // n from eq. (2)
    vec3i_t* normal;
} plane_t;

//-------------------------------------------------------------------------------------------------------------
// Cube
//-------------------------------------------------------------------------------------------------------------
/**
 * @brief Allocates and sets a cube
 *
 * @param cx x-coordinate of the center
 * @param cy y-coordinate of the center
 * @param cz z-coordinate of the center
 * @param size lenght of one side
 *
 * @return A pointer to the newly constructed cube
 */
shape_t*    obj_cube_new               (int cx, int cy, int cz, int width, int height, int type);
void        obj_cube_rotate            (shape_t* cube, float angle_x_rad, float angle_y_rad, float angle_z_rad);
void        obj_cube_free              (shape_t* cube);

//-------------------------------------------------------------------------------------------------------------
// Ray
//-------------------------------------------------------------------------------------------------------------
// the pixel in the screen where the ray points to
/**
 * @brief Allocates and sets a ray structure starting from the origin to the point (x, y, z)
 *
 * @param x x-coordinate of ray's destination
 * @param y y-coordinate of ray's destination
 * @param z z-coordinate of ray's destination
 *
 * @return A pointer to the newly constructed ray
 */
ray_t*      obj_ray_new                (int x, int y, int z);
/**
 * @brief Sets the destination (`end` member) of a ray
 *
 * @param ray Pointer to the ray to modify
 * @param x x-coordinate of ray's new destination
 * @param y y-coordinate of ray's new sestination
 * @param z z-coordinate of ray's new destination
 */
void        obj_ray_send               (ray_t* ray, int x, int y, int z); 
void        obj_ray_free               (ray_t* ray);

//-------------------------------------------------------------------------------------------------------------
// Plane
//-------------------------------------------------------------------------------------------------------------
/**
 * @brief Allocates a plane and sets its normal vector and offset given three points
 *
 * @param p0 First point on the plane
 * @param p1 Second point on the plane
 * @param p2 Third point on the plane
 *
 * @return A pointer to the newly constructed plane
 */
plane_t*    obj_plane_new              (vec3i_t* p0, vec3i_t* p1, vec3i_t* p2);
vec3i_t     obj_ray_plane_intersection (plane_t* plane, ray_t* ray);
bool        obj_ray_hits_rectangle     (ray_t* ray, vec3i_t* p0, vec3i_t* p1, vec3i_t* p2, vec3i_t* p3);
bool obj_ray_hits_triangle(ray_t* ray, vec3i_t* p0, vec3i_t* p1, vec3i_t* p2);
/* find the z-coordinate on a plane give x and y */
static inline int
            obj_plane_z_at_xy          (plane_t* plane, int x, int y) {
    // solve for z in plane's eq/n: n.x*x + n.y*y + n.z*z + offset = 0
    vec3i_t coeffs = (vec3i_t) {plane->normal->x, plane->normal->y, plane->offset};
    vec3i_t xyz = (vec3i_t) {x, y, 1};
    return round(1.0/plane->normal->z*(-vec_vec3i_dotprod(&coeffs, &xyz)));
}
/* recompute plane's normal and offset given 3 points */
void        obj_plane_set              (plane_t* plane, vec3i_t* p0, vec3i_t* p1, vec3i_t* p2);
void        obj_plane_free             (plane_t* plane);

#endif /* OBJECTS_H */
