#include "../base/tuple.h"
#include "../solids/solid.h"
#include "../base/transforms.h"

#include "test.h"

#pragma once

typedef struct  {
    Ray *saved_ray;
} shape_state;

void test_free_func(void* data) {
    if (((shape_state*)data)->saved_ray != NULL)
        free_ray(((shape_state*)data)->saved_ray);
    free(data);
}

IntersectionArray *test_solid_intersect(Solid *s, const Ray *r) {
    shape_state* state = (shape_state*)s->data;
    state->saved_ray = copy_ray(r);
    return NULL;
}

Tuple *test_solid_normal(const Solid *s, const Tuple *point) {
    shape_state* state = (shape_state*)s->data;
    return copy_tuple(point);
}

Solid *test_shape() {
    Solid *s = solid();
    s->data = malloc(sizeof(shape_state));
    ((shape_state*)s->data)->saved_ray = NULL;
    s->intersect_func = test_solid_intersect;
    s->normal_at_func = test_solid_normal;
    s->free_data_func = test_free_func;
    return s;
}

void TestSolidInterfaceScale() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Solid *s = test_shape();
    set_transform(s, scaling(2,2,2));
    intersect(s, r);

    Tuple origin = {0,0,-2.5,1};
    Tuple direction = {0,0,0.5};
    shape_state *saved = (shape_state*)s->data;
    TupleEqual(saved->saved_ray->origin, &origin);
    TupleEqual(saved->saved_ray->direction, &direction);
    free_solid(s); free_ray(r);
}

void TestSolidInterfaceTranslation() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Solid *s = test_shape();
    set_transform(s, translation(5,0,0));
    intersect(s, r);

    Tuple origin = {-5,0,-5,1};
    Tuple direction = {0,0,1};
    shape_state *saved = (shape_state*)s->data;
    TupleEqual(saved->saved_ray->origin, &origin);
    TupleEqual(saved->saved_ray->direction, &direction);
    free_solid(s); free_ray(r);
}

void TestSolidInterfaceNormal() {
    Solid *s = test_shape();
    Tuple point = {0, 1.70711, -0.70711, 1};
    set_transform(s, translation(0,1,0));

    Tuple *res = normal_at(s, &point);
    Tuple exp = {0, 0.70711, -0.70711};
    TupleEqual(res, &exp);
    free_solid(s); free(res);
}

void TestSolidInterfaceNormalTransform() {
    Solid *s = test_shape();
    Tuple point = {0, sqrt(2)/2, -sqrt(2)/2, 1};
    set_transform(s, chain_matmul(2, rotation_z(M_PI/5), scaling(1, 0.5, 1)));

    Tuple *res = normal_at(s, &point);
    Tuple exp = {0, 0.97014, -0.24254};
    TupleEqual(res, &exp);
    free_solid(s); free(res);
}

void TestSolidFeature() {
    Feature f = {"Solid Interface"};
    AddTest(&f, TestSolidInterfaceScale, "Intersecting a scaled shape with a ray");
    AddTest(&f, TestSolidInterfaceTranslation, "Intersecting a translated shape with a ray");
    AddTest(&f, TestSolidInterfaceNormal, "Computing the normal on a translated shape");
    AddTest(&f, TestSolidInterfaceNormalTransform, "Computing the normal on a transformed shape");
    AddFeature(f);
}
