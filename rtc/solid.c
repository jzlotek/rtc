#include "../rtc.h"

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
    world_normal->vals[3] = 0;

    free_matrix(transform_inv);
    return norm(world_normal);
}


Pattern *pattern_conatainer() {
    Pattern *pattern = (Pattern*)malloc(sizeof(Pattern));
    return pattern;
}

void free_stripe_pattern(void *p) {
    StripePattern *pattern = (StripePattern*)p;
    free(pattern->a);
    free(pattern->b);
    free(pattern);
}

Tuple *stripe_at_helper(void *p, const Tuple *point) {
    StripePattern *pattern = (StripePattern*)p;
    if ((int)floor(point->vals[0]) % 2 == 0) {
        return pattern->a;
    }
    return pattern->b;
}

Pattern *stripe_pattern(Tuple *a, Tuple *b) {
    Pattern *p = pattern_conatainer();
    StripePattern *sp = (StripePattern*)malloc(sizeof(StripePattern));
    sp->a = a;
    sp->b = b;
    p->pattern_p = sp;
    p->free_func = free_stripe_pattern;
    p->stripe_at_func = stripe_at_helper;
    return p;
}

void free_pattern(Pattern *pattern) {
    pattern->free_func(pattern->pattern_p);
    if (pattern->transform != NULL) {
        free_matrix(pattern->transform);
    }
    free(pattern);
}

Tuple *stripe_at(Pattern *pattern, const Tuple *point) {
    return pattern->stripe_at_func(pattern->pattern_p, point);
}

void set_pattern_transform(Pattern *pattern, Matrix *transform) {
    if (pattern->transform != NULL) {
        free_matrix(pattern->transform);
    }
    pattern->transform = transform;
}

Tuple *stripe_at_object(Pattern *pattern, Solid *object, Tuple *pt) {
    Matrix *m = inverse(object->transform);
    Tuple *object_point = apply(copy_tuple(pt), m);
    free_matrix(m);
    m = inverse(pattern->transform);
    Tuple *pattern_point = apply(object_point, m);
    free_matrix(m);
    Tuple *ret = stripe_at(pattern, pattern_point);
    free(pattern_point);

    return ret;
}