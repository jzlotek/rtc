#include "tuple.h"
#include "matrix.h"
#include "../utils/consts.h"
#include "../utils/funcs.h"

#pragma once

typedef struct {
    Tuple *origin;
    Tuple *direction;
} Ray;

typedef struct {
    float t;
    void *solid;
} __intersection;

typedef __intersection* Intersection;

Ray *ray(float start_x, float start_y, float start_z, float d_x, float d_y, float d_z);
Intersection intersection(float t, void *solid);
Ray *copy_ray(const Ray *src);
void free_ray(Ray *r);
Tuple *position(Ray *r, float t);
Ray *transform(const Ray *r, const Matrix *m);

Ray *ray(float start_x, float start_y, float start_z, float d_x, float d_y, float d_z) {
    Ray *r = (Ray*)malloc(sizeof(Ray));
    r->origin = point(start_x, start_y, start_z);
    r->direction = vec(d_x, d_y, d_z);
    return r;
}

Intersection intersection(float t, void *solid) {
    Intersection i = (Intersection)malloc(sizeof(__intersection));
    i->solid = solid;
    i->t = t;
    return i;
}

Ray *copy_ray(const Ray *src) {
    Ray *ret = (Ray*)malloc(sizeof(Ray));
    ret->origin = copy_tuple(src->origin);
    ret->direction = copy_tuple(src->direction);
    return ret;
}

void free_ray(Ray *r) {
    free(r->direction);
    free(r->origin);
    free(r);
}

Tuple *position(Ray *r, float t) {
    Tuple *ret = copy_tuple(r->origin);
    Tuple *d_t = copy_tuple(r->direction);
    mult(d_t, t);
    add(ret, d_t);
    free(d_t);
    return ret;
}

Ray *transform(const Ray *r, const Matrix *m) {
    Ray *newRay = copy_ray(r);
    newRay->origin = apply(newRay->origin, m);
    newRay->direction = apply(newRay->direction, m);
    return newRay;
}