#include "../base/ray.h"
#include "../base/matrix.h"
#include "../utils/funcs.h"
#include "../base/light.h"

#include "solid.h"

#pragma once


Solid *plane();
IntersectionArray *plane_intersect(Solid *sphere, const Ray *r);
Tuple *plane_normal_at(const Solid *s, const Tuple *pos);

Solid *plane() {
    Solid *s = solid();
    s->type = PLANE;
    s->normal_at_func = plane_normal_at;
    s->intersect_func = plane_intersect;
    return s;
}

IntersectionArray *plane_intersect(Solid *plane, const Ray *r) {
    IntersectionArray *arr = Intersection_array();
    Tuple z = {0,0,0,1};
    Tuple *oc = sub(copy_tuple(r->origin), &z);
    float a = dot(r->direction, r->direction);
    float b = 2.0 * dot(oc, r->direction);
    float c = dot(oc, oc) - 1; // 1 is radius * radius (unit sphere so r == 1)
    float disc = (b * b) - (4 * a * c);
    if (disc >= 0) {
        Intersection_arr_add(arr, intersection((-b - sqrt(disc)) / 2.0 / a, plane));
        Intersection_arr_add(arr, intersection((-b + sqrt(disc)) / 2.0 / a, plane));
    }
    free(oc);
    return arr;
}


Tuple *plane_normal_at(const Solid *s, const Tuple *pos) {
    Tuple *world_point = copy_tuple(pos);
    Matrix *transform_inv = inverse(s->transform);
    Tuple z = {0,0,0,1};
    sub(apply(world_point, transform_inv), &z);
    Tuple *world_normal = apply(world_point, transpose(transform_inv));
    world_normal->w = 0;

    free_matrix(transform_inv);
    return norm(world_normal);
}
