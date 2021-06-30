#include "../base/ray.h"
#include "../base/matrix.h"
#include "../utils/funcs.h"
#include "../base/light.h"

#include "solid.h"

#pragma once


Solid *sphere();
IntersectionArray *sphere_intersect(Solid *sphere, const Ray *r);
Tuple *sphere_normal_at(const Solid *s, const Tuple *pos);

Solid *sphere() {
    Solid *s = solid();
    s->type = SPHERE;
    s->normal_at_func = sphere_normal_at;
    s->intersect_func = sphere_intersect;
    return s;
}

IntersectionArray *sphere_intersect(Solid *sphere, const Ray *r) {
    IntersectionArray *arr = Intersection_array();
    Tuple z = {0,0,0,1};
    Tuple *oc = sub(copy_tuple(r->origin), &z);
    float a = dot(r->direction, r->direction);
    float b = 2.0 * dot(oc, r->direction);
    float c = dot(oc, oc) - 1; // 1 is radius * radius (unit sphere so r == 1)
    float disc = (b * b) - (4 * a * c);
    if (disc >= 0) {
        Intersection_arr_add(arr, intersection((-b - sqrt(disc)) / 2.0 / a, sphere));
        Intersection_arr_add(arr, intersection((-b + sqrt(disc)) / 2.0 / a, sphere));
    }
    free(oc);
    return arr;
}


Tuple *sphere_normal_at(const Solid *s, const Tuple *pos) {
    return copy_tuple(pos);
}
