#include "../utils/consts.h"
#include "../utils/funcs.h"
#include "../base/matrix.h"
#include "../base/light.h"
#include "../base/ray.h"

#pragma once

typedef struct solid Solid;

typedef struct {
    float t;
    Solid *solid;
} __intersection;
typedef __intersection* Intersection;

TEMPLATE_ARRAY(Intersection);

struct solid {
    ObjectType type;
    Matrix *transform;
    Material *material;
    void *data;
    IntersectionArray *(*intersect_func)(Solid *solid, const Ray *r);
    Tuple *(*normal_at_func)(const Solid *solid, const Tuple *pos);
    void (*free_data_func)(void*);
};

Solid *solid();
void set_transform(Solid *solid, Matrix *m);
void set_material(Solid *solid, Material *m);
IntersectionArray *intersect(Solid *solid, const Ray *r);
Tuple *normal_at(const Solid *s, const Tuple *pos);
void clean_Intersection_array(IntersectionArray *ia);
Intersection intersection(float t, Solid *solid);

Intersection intersection(float t, Solid *solid) {
    Intersection i = (Intersection)malloc(sizeof(__intersection));
    i->solid = solid;
    i->t = t;
    return i;
}

void clean_Intersection_array(IntersectionArray *ia) {
    for (unsigned int i = 0; i < ia->length; i++) {
        free(ia->data[i]);
    }
}

int __cmp_Intersection(const void *a, const void *b) {
    float v1 = (*(Intersection*)a)->t;
    float v2 = (*(Intersection*)b)->t;
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    return 0;
}

void sort_IntersectionArray(IntersectionArray *arr) {
    qsort(arr->data, arr->length, sizeof(Intersection), __cmp_Intersection);
}

Intersection hit(IntersectionArray *arr) {
    Intersection ret = intersection(-1, NULL);
    sort_IntersectionArray(arr);
    for (unsigned int i = 0; i < arr->length; i++) {
        Intersection v = get_Intersection_array(arr, i);
        if (v->t >= 0) {
            ret->t = v->t;
            ret->solid = v->solid;
            return ret;
        }
    }
    return ret;
}

Solid *solid() {
    Solid *s = (Solid*)malloc(sizeof(Solid));
    s->material = material();
    s->transform = I();
    s->free_data_func = NULL;
    return s;
}

void free_solid(Solid *s) {
    free_material(s->material);
    free_matrix(s->transform);
    if (s->free_data_func != NULL) {
        s->free_data_func(s->data);
    }
    free(s);
}

void set_transform(Solid *solid, Matrix *m) {
    free_matrix(solid->transform);
    solid->transform = m;
}

IntersectionArray *intersect(Solid *solid, const Ray *ra) {
    Matrix *t_inv = inverse(solid->transform);
    Ray *r = transform(ra, t_inv);
    IntersectionArray *ia = solid->intersect_func(solid, r);
    free_matrix(t_inv); free_ray(r);
    return ia;
}

void set_material(Solid *solid, Material *m) {
    free_material(solid->material);
    solid->material = m;
}

Tuple *normal_at(const Solid *s, const Tuple *pos) {
    Tuple *world_point = copy_tuple(pos);
    Matrix *transform_inv = inverse(s->transform);
    Tuple z = {0,0,0,1};
    sub(apply(world_point, transform_inv), &z);
    Tuple *world_normal = s->normal_at_func(s, world_point);
    free(world_point);
    world_normal = apply(world_normal, transpose(transform_inv));
    world_normal->w = 0;

    free_matrix(transform_inv);
    return norm(world_normal);
}
