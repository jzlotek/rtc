#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __x86_64__
   #include <immintrin.h>
#else
  #include "rtc/sse2neon.h"
#endif

#include "rtc/consts.h"
#include "rtc/funcs.h"

#ifndef __RTC_H__
#define __RTC_H__


/* Structs */
typedef union {
    float vals[4];
    __m128 vec;
} Tuple;

typedef struct {
    float **data;
    unsigned int rows;
    unsigned int cols;
} Matrix;

typedef struct {
    unsigned int width;
    unsigned int height;
    Tuple **__canvas;
} Canvas;

typedef struct solid Solid;
typedef Solid* SolidP;
typedef struct light Light;
typedef Light* LightP;

typedef struct {
    float t;
    Solid *solid;
} __intersection;

typedef __intersection* Intersection;

#ifndef __COMPLEX_ARRAYS__
#define __COMPLEX_ARRAYS__
TEMPLATE_ARRAY(SolidP);
TEMPLATE_ARRAY(LightP);
TEMPLATE_ARRAY(Intersection);
#endif

typedef struct world {
    LightPArray *lights;
    SolidPArray *objects;
} World;

typedef struct computations {
    Solid* obj;
    float t;
    bool inside;
    Tuple *point;
    Tuple *eyev;
    Tuple *normalv;
    Tuple *over_point;
} WorldComputation;

typedef struct {
    unsigned int hsize;
    unsigned int vsize;
    float field_of_view;
    float half_width;
    float half_height;
    float pixel_size;
    Matrix *transform;
} Camera;

typedef struct {
    Tuple *origin;
    Tuple *direction;
} Ray;

typedef enum { Stripe, } PatternType;

typedef struct {
    PatternType type;
    void *pattern_p;
    void (*free_func)(void *);
    Tuple *(*stripe_at_func)(void *pattern, const Tuple *point);
    Matrix *transform;
} Pattern;



typedef struct light {
    Tuple *intensity;
    Tuple *position;
} Light;

typedef struct {
    Tuple *color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    Pattern *pattern;
} Material;

struct solid {
    ObjectType type;
    Matrix *transform;
    Material *material;
    void *data;
    IntersectionArray *(*intersect_func)(Solid *solid, const Ray *r);
    Tuple *(*normal_at_func)(const Solid *solid, const Tuple *pos);
    void (*free_data_func)(void*);
};

/* Tuple */

bool isPoint(Tuple *t);
bool isVector(Tuple *t);
void print_tuple(Tuple *t);
Tuple *copy_tuple(const Tuple *src);
Tuple *copy_from(Tuple *dest, const Tuple *src);
bool equal(const Tuple *t1, const Tuple *t2);
Tuple *point(float x, float y, float z);
Tuple *vec(float x, float y, float z);
Tuple *add(Tuple *t1, const Tuple *t2);
Tuple *sub(Tuple *t1, const Tuple *t2);
Tuple *neg(Tuple *t1);
Tuple *mult(Tuple *t1, float c);
Tuple *divide(Tuple *t1, float c);
Tuple *prod(Tuple *t1, const Tuple *t2);
Tuple *cross(const Tuple *t1, const Tuple *t2);
float dot(const Tuple *t1, const Tuple *t2);
Tuple *norm(Tuple *t);
float magnitude(const Tuple *t);
Tuple *reflect(Tuple *a, const Tuple *b);

/* Matrix */

Matrix *matrix(float data[4][4], unsigned int rows, unsigned int cols);
void free_matrix(Matrix *m);
Matrix *copy(const Matrix *src);
Tuple *apply(Tuple *point, const Matrix *transform);
bool mateq(const Matrix *m1, const Matrix *m2);
Matrix *matmul(const Matrix *m1, const Matrix *m2);
Matrix *I();
Matrix *transpose(Matrix *m);
Matrix *submatrix(const Matrix *a, unsigned int r, unsigned int c);
float det(const Matrix *a);
float minor(const Matrix *a, unsigned int i, unsigned int j);
float cofactor(const Matrix *a, unsigned int i, unsigned int j);
Matrix *inverse(const Matrix *m);

/* Transform */

Matrix *translation(float x, float y, float z);
Matrix *scaling(float x, float y, float z);
Matrix *rotation_x(float r);
Matrix *rotation_y(float r);
Matrix *rotation_z(float r);
Matrix *shear(float xy, float xz, float yx, float yz, float zx, float zy);
Matrix *chain_matmul(int count, ...);
Matrix *view_transform(Tuple *from, Tuple *to, Tuple *up);

/* Canvas */

Canvas *canvas(unsigned int width, unsigned int height);
void free_canvas(Canvas *c);
void write_pixel(Canvas *c, unsigned int x, unsigned int h, const Tuple *color);
Tuple *pixel_at(Canvas *c, unsigned int x, unsigned int y);
void canvas_to_ppm(Canvas *c, FILE *stream);

/* World */

void sort_IntersectionArray(IntersectionArray *arr);
Intersection hit(IntersectionArray *arr);
void clean_Solid_array(SolidPArray *ar);
World *world();
World *default_world();
void add_object(World *w, SolidP o);
void add_light(World *w, Light *l);
Solid *get_object(World *w, unsigned int i);
Light *get_light(World *w, unsigned int i);
void free_world(World *w);
IntersectionArray *intersect_world(World *w, Ray *r);
WorldComputation *world_computation(Solid *o, float t, Tuple *point, Tuple *eyev, Tuple *normalv);
void free_world_computation(WorldComputation *wc);
WorldComputation *prepare_computations(Intersection i, Ray *r);
Tuple *shade_hit(World *w, WorldComputation *wc);
Tuple *color_at(World *w, Ray *r);
bool is_shadowed(World *w, Tuple *point);

/* Camera */

Camera *camera(unsigned int hsize, unsigned int vsize, float field_of_view);
void free_camera(Camera *c);
Camera *set_camera_transform(Camera *c, Matrix *t);
Ray *ray_for_pixel(Camera *c, int x, int y);
Canvas *render(Camera *c, World *world);

/* Ray */

Ray *ray(float start_x, float start_y, float start_z, float d_x, float d_y, float d_z);
Ray *copy_ray(const Ray *src);
void free_ray(Ray *r);
Tuple *position(Ray *r, float t);
Ray *transform(const Ray *r, const Matrix *m);

/* Light */

Light *point_light(Tuple *intensity, Tuple *position);
void free_point_light(Light *light);
Material *material();
Material *copy_material(const Material *m);
void set_color(Material *m, Tuple *color);
void free_material(Material *m);
Tuple *lighting(const Material *m, const Solid *object, const Light *l, const Tuple *point, const Tuple *eyev, const Tuple *normalv, bool in_shadow);

/* Solid */

Solid *solid();
void set_transform(Solid *solid, Matrix *m);
void set_material(Solid *solid, Material *m);
IntersectionArray *intersect(Solid *solid, const Ray *r);
Tuple *normal_at(const Solid *s, const Tuple *pos);
void clean_Intersection_array(IntersectionArray *ia);
Intersection intersection(float t, Solid *solid);
void free_solid(Solid *s);

/* Sphere */

Solid *sphere();
IntersectionArray *sphere_intersect(Solid *sphere, const Ray *r);
Tuple *sphere_normal_at(const Solid *s, const Tuple *pos);

/* Pattern */

typedef struct {
    Tuple *a;
    Tuple *b;
} StripePattern;

Pattern *stripe_pattern(Tuple *a, Tuple *b);
void free_pattern(Pattern *pattern);
Tuple *stripe_at(Pattern *pattern, const Tuple *point);
Pattern *pattern_container();
Tuple *stripe_at_object(Pattern *pattern, Solid *object, Tuple *pt);
void set_pattern_transform(Pattern *p, Matrix *transform);

/* Plane */

Solid *plane();
IntersectionArray *plane_intersect(Solid *sphere, const Ray *r);
Tuple *plane_normal_at(const Solid *s, const Tuple *pos);



#endif
