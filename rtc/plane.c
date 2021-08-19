#include "../rtc.h"

IntersectionArray *plane_intersect(Solid *plane, const Ray *r) {
    IntersectionArray *arr = Intersection_array();
    if (fabs(r->direction->vals[1]) < EPSILON) {
        return arr;
    }

    float t = -r->origin->vals[1] / r->direction->vals[1];
    Intersection_arr_add(arr, intersection(t, plane));

    return arr;
}


Tuple *plane_normal_at(const Solid *s, const Tuple *pos) {
    return vec(0, 1, 0);
}

Solid *plane() {
    Solid *s = solid();
    s->type = PLANE;
    s->normal_at_func = plane_normal_at;
    s->intersect_func = plane_intersect;
    return s;
}
