#include "../solids/sphere.h"
#include "../base/transforms.h"
#include "test.h"


void TestSphereDefault() {
    Sphere *s = sphere();
    Matrix *ident = I();
    True(mateq(s->transform, ident));

    free(ident); free_sphere(s);
}

void TestSphereTransform() {
    Sphere *s = sphere();
    Matrix *exp = translation(2, 3, 4);
    set_transform(s, translation(2, 3, 4));
    True(mateq(s->transform, exp));

    free_sphere(s); free(exp);
}

void TestSphereIntersect() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Sphere *s = sphere();
    set_transform(s, scaling(2, 2, 2));
    IntersectionArray *arr = intersect(s, r);
    if (Equal(arr->length, 2)) {
        Equal(get_Intersection_array(arr, 0).t, 3);
        Equal(get_Intersection_array(arr, 1).t, 7);
    }

    free_ray(r); free_Intersection_array(arr); free_sphere(s);
}

void TestSphereIntersectMiss() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Sphere *s = sphere();
    set_transform(s, translation(5, 0, 0));
    IntersectionArray *arr = intersect(s, r);
    Equal(arr->length, 0);

    free_ray(r); free_Intersection_array(arr); free_sphere(s);
}

void  TestSphere() {
  Feature f = {"Spheres"};
  AddTest(&f, TestSphereDefault, "A sphere's default transformation");
  AddTest(&f, TestSphereTransform, "Changing the sphere's transform");
  AddTest(&f, TestSphereIntersect, "Intersecting a scaled sphere with a ray");
  AddTest(&f, TestSphereIntersectMiss, "Intersecting a translated sphere with a ray");
  AddFeature(f);
}