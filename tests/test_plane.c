#include "../solids/plane.h"
#include "../base/transforms.h"
#include "test.h"


void TestPlaneDefault() {
    Solid *s = plane();
    Tuple p1 = {0,0,0,1}, p2 = {10,0,-10,1}, p3 = {-5,0,150,1};
    Tuple n1 = {0,1,0}, n2 = {0,1,0}, n3 = {0,1,0};

    Tuple *r1 = s->normal_at_func(s, &p1);
    Tuple *r2 = s->normal_at_func(s, &p2);
    Tuple *r3 = s->normal_at_func(s, &p3);

    TupleEqual(r1, &n1);
    TupleEqual(r2, &n2);
    TupleEqual(r3, &n3);

    free_solid(s);
    free(r1); free(r2); free(r3);
}

void TestPlaneParallelIntersect() {
    Solid *s = plane();

    Ray *r = ray(0, 10, 0, 0, 0, 1);
    IntersectionArray *ia = s->intersect_func(s, r);
    Equal(ia->length, 0);

    clean_Intersection_array(ia); free_Intersection_array(ia);
    free_solid(s); free_ray(r);
}

void TestPlaneCoplanarIntersect() {
    Solid *s = plane();

    Ray *r = ray(0, 0, 0, 0, 0, 1);
    IntersectionArray *ia = s->intersect_func(s, r);
    Equal(ia->length, 0);

    clean_Intersection_array(ia); free_Intersection_array(ia);
    free_solid(s); free_ray(r);
}

void TestPlaneAboveIntersect() {
    Solid *s = plane();

    Ray *r = ray(0, 1, 0, 0, -1, 0);
    IntersectionArray *ia = s->intersect_func(s, r);
    if (Equal(ia->length, 1)) {
        Equal(get_Intersection_array(ia, 0)->t, 1);
        Equal(*(float*)get_Intersection_array(ia, 0)->solid, *(float*)s);
    }

    clean_Intersection_array(ia); free_Intersection_array(ia);
    free_solid(s); free_ray(r);
}

void TestPlaneBelowIntersect() {
    Solid *s = plane();

    Ray *r = ray(0, -1, 0, 0, 1, 0);
    IntersectionArray *ia = s->intersect_func(s, r);
    if (Equal(ia->length, 1)) {
        Equal(get_Intersection_array(ia, 0)->t, 1);
        Equal(*(float*)get_Intersection_array(ia, 0)->solid, *(float*)s);
    }

    clean_Intersection_array(ia); free_Intersection_array(ia);
    free_solid(s); free_ray(r);
}


void TestPlaneFeature() {
  Feature f = {"Planes"};
  AddTest(&f, TestPlaneDefault, "The normal of a plane is constant everywhere");
  AddTest(&f, TestPlaneParallelIntersect, "Intersect with a ray parallel to the plane");
  AddTest(&f, TestPlaneCoplanarIntersect, "Intersect with a coplanar ray");
  AddTest(&f, TestPlaneAboveIntersect, "A ray intersecting a plane from above");
  AddTest(&f, TestPlaneBelowIntersect, "A ray intersecting a plane from below");
  AddFeature(f);
}