#include "../base/ray.h"
#include "../base/matrix.h"
#include "../utils/funcs.h"

#pragma once

typedef struct {
    float radius;
    Tuple *center;
    Matrix *transform;
} Sphere;

TEMPLATE_ARRAY(Intersection);

Sphere *sphere();
void free_sphere(Sphere *sphere);
void set_transform(Sphere *sphere, Matrix *m);
IntersectionArray *intersect(Sphere *sphere, const Ray *r);
Intersection intersection(float t, Sphere *s);

Sphere *sphere() {
    Sphere *s = (Sphere*)malloc(sizeof(Sphere));
    s->radius = 1.0;
    s->center = point(0, 0, 0);
    s->transform = I();
    return s;
}

void free_sphere(Sphere *sphere) {
    free(sphere->transform);
    free(sphere->center);
    free(sphere);
}

void set_transform(Sphere *sphere, Matrix *m) {
    if (sphere->transform != NULL) free(sphere->transform);
    sphere->transform = m;
}

IntersectionArray *intersect(Sphere *sphere, const Ray *ra) {
    Matrix *t_inv = inverse(sphere->transform);
    Ray *r = transform(ra, t_inv);
    free(t_inv);
    IntersectionArray *arr = Intersection_array();
    Tuple *oc = sub(copy_tuple(r->origin), sphere->center);
    float a = dot(r->direction, r->direction);
    float b = 2.0 * dot(oc, r->direction);
    float c = dot(oc, oc) - sphere->radius * sphere->radius;
    float disc = (b * b) - (4 * a * c);
    if (disc >= 0) {
        Intersection in = {(-b - sqrt(disc)) / 2.0 / a, sphere};
        Intersection_arr_add(arr, in);
        in.t = (-b + sqrt(disc)) / 2.0 / a;
        Intersection_arr_add(arr, in);
    }
    free(oc);
    free_ray(r);
    return arr;
}

Intersection intersection(float t, Sphere *s) {
    Intersection i = {t, s};
    return i;
}

int __cmp_Intersection(const void *a, const void *b) {
    Intersection *v1 = (Intersection*)a;
    Intersection *v2 = (Intersection*)b;
    return v1->t - v2->t;
}

void sort_IntersectionArray(IntersectionArray *arr) {
    qsort(arr->data, arr->length, sizeof(Intersection), __cmp_Intersection);
}

Intersection hit(IntersectionArray *arr) {
    Intersection ret = {-1, NULL};
    sort_IntersectionArray(arr);
    for (unsigned int i = 0; i < arr->length; i++) {
        Intersection v = get_Intersection_array(arr, i);
        if (v.t >= 0 && ret.solid == NULL) {
            return v;
        }
    }
    return ret;
}
