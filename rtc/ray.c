#include "../rtc.h"

Ray *ray(float start_x, float start_y, float start_z, float d_x, float d_y, float d_z) {
    Ray *r = (Ray*)malloc(sizeof(Ray));
    r->origin = point(start_x, start_y, start_z);
    r->direction = vec(d_x, d_y, d_z);
    return r;
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
