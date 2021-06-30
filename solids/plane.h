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
    if (fabs(r->direction->y) < EPSILON) {
        return arr;
    }

    float t = -r->origin->y / r->direction->y;
    Intersection_arr_add(arr, intersection(t, plane));

    return arr;
}


Tuple *plane_normal_at(const Solid *s, const Tuple *pos) {
    return vec(0, 1, 0);
}
