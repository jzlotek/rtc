#include <math.h>
#include "../base/ray.h"
#include "../utils/consts.h"
#include "../solids/sphere.h"
#include "test.h"

void TestPosition() {
  Ray *r = ray(2, 3, 4, 1, 0, 0);
  Tuple *exp = point(2, 3, 4);
  Tuple *res = position(r, 0);
  TupleEqual(res, exp);
  free(exp); free(res);
  exp = point(1, 3, 4);
  res = position(r, -1);
  TupleEqual(res, exp);
  free(exp); free(res); free_ray(r);
}

void TestSphereIntersection() {
  Ray *r = ray(0, 0, -5, 0, 0, 1);
  Sphere *s = sphere();
  IntersectionArray *points = intersect(s, r);
  if (Equal(points->length, 2)) {
    Equal(get_Intersection_array(points, 0)->t, 4);
    Equal(get_Intersection_array(points, 1)->t, 6);
  }
  clean_Intersection_array(points); free_ray(r); free_Intersection_array(points); free_sphere(s);
}

void TestSphereIntersectionTangent() {
  Ray *r = ray(0, 1, -5, 0, 0, 1);
  Sphere *s = sphere();
  IntersectionArray *points = intersect(s, r);
  if (Equal(points->length, 2)) {
    Equal(get_Intersection_array(points, 0)->t, 5);
    Equal(get_Intersection_array(points, 1)->t, 5);
  }
  clean_Intersection_array(points); free_ray(r); free_Intersection_array(points); free_sphere(s);
}

void TestSphereIntersectionMisses() {
  Ray *r = ray(0, 2, -5, 0, 0, 1);
  Sphere *s = sphere();
  IntersectionArray *points = intersect(s, r);
  Equal(points->length, 0);
  clean_Intersection_array(points); free_ray(r); free_Intersection_array(points); free_sphere(s);
}

void TestSphereIntersectionInside() {
  Ray *r = ray(0, 0, 0, 0, 0, 1);
  Sphere *s = sphere();
  IntersectionArray *points = intersect(s, r);
  if (Equal(points->length, 2)) {
    Equal(get_Intersection_array(points, 0)->t, -1);
    Equal(get_Intersection_array(points, 1)->t, 1);
  }
  clean_Intersection_array(points); free_ray(r); free_Intersection_array(points); free_sphere(s);
}

void TestSphereIntersectionOutside() {
  Ray *r = ray(0, 0, 5, 0, 0, 1);
  Sphere *s = sphere();
  IntersectionArray *points = intersect(s, r);
  if (Equal(points->length, 2)) {
    Equal(get_Intersection_array(points, 0)->t, -6);
    Equal(get_Intersection_array(points, 1)->t, -4);
  }
  clean_Intersection_array(points); free_ray(r); free_Intersection_array(points); free_sphere(s);
}

void TestRayIntersections() {
  IntersectionArray *intersections = Intersection_array();
  Sphere *s = sphere();
  Intersection_arr_add(intersections, intersection(1, s));
  Intersection_arr_add(intersections, intersection(2, s));
  Equal(intersections->length, 2);
  Equal(get_Intersection_array(intersections, 0)->t, 1);
  Equal(get_Intersection_array(intersections, 1)->t, 2);
  Equal(*(float*)get_Intersection_array(intersections, 0)->solid, *(float*)get_Intersection_array(intersections, 1)->solid);

  clean_Intersection_array(intersections); free_Intersection_array(intersections); free_sphere(s);
}

void TestRayHit() {
  IntersectionArray *intersections = Intersection_array();
  Sphere *s = sphere();
  Intersection v1 = intersection(1, s);
  Intersection v2 = intersection(2, s);
  Intersection_arr_add(intersections, v1);
  Intersection_arr_add(intersections, v2);
  Intersection h = hit(intersections);

  Equal(h->t, v1->t);
  Equal(*(float*)h->solid, *(float*)v1->solid);

  free(h); clean_Intersection_array(intersections); free_Intersection_array(intersections); free_sphere(s);
}

void TestRayHitOneNegative() {
  IntersectionArray *intersections = Intersection_array();
  Sphere *s = sphere();
  Intersection v1 = intersection(-1, s);
  Intersection v2 = intersection(1, s);
  Intersection_arr_add(intersections, v1);
  Intersection_arr_add(intersections, v2);
  Intersection h = hit(intersections);

  Equal(h->t, v2->t);
  True(h->solid == v2->solid);

  free(h); clean_Intersection_array(intersections); free_Intersection_array(intersections); free_sphere(s);
}

void TestRayHitAllNegative() {
  IntersectionArray *intersections = Intersection_array();
  Sphere *s = sphere();
  Intersection v1 = intersection(-1, s);
  Intersection v2 = intersection(-2, s);
  Intersection_arr_add(intersections, v1);
  Intersection_arr_add(intersections, v2);
  Intersection h = hit(intersections);

  True(h->solid == NULL);

  free(h); clean_Intersection_array(intersections); free_Intersection_array(intersections); free_sphere(s);
}

void TestRayHitOutOfOrder() {
  IntersectionArray *intersections = Intersection_array();
  Sphere *s = sphere();
  Intersection v1 = intersection(5, s);
  Intersection v2 = intersection(7, s);
  Intersection v3 = intersection(-3, s);
  Intersection v4 = intersection(2, s);
  Intersection_arr_add(intersections, v1);
  Intersection_arr_add(intersections, v2);
  Intersection_arr_add(intersections, v3);
  Intersection_arr_add(intersections, v4);
  Intersection h = hit(intersections);

  True(h->solid == s);
  Equal(h->t, 2);

  free(h); clean_Intersection_array(intersections); free_Intersection_array(intersections); free_sphere(s);
}

void TestRayTranslation() {
  Ray *r = ray(1,2,3,0,1,0);
  Tuple *origin = point(4, 6, 8);
  Tuple *direction = vec(0, 1, 0);
  Matrix *m = translation(3, 4, 5);
  Ray *res = transform(r, m);

  TupleEqual(res->origin, origin);
  TupleEqual(res->direction, direction);

  free_ray(r); free(origin); free(direction); free_ray(res); free_matrix(m);
}

void TestRayScaling() {
  Ray *r = ray(1,2,3,0,1,0);
  Tuple *origin = point(2, 6, 12);
  Tuple *direction = vec(0, 3, 0);
  Matrix *m = scaling(2, 3, 4);
  Ray *res = transform(r, m);

  TupleEqual(res->origin, origin);
  TupleEqual(res->direction, direction);

  free_ray(r); free(origin); free(direction); free_ray(res); free_matrix(m);
}

void TestRay() {
  Feature f = {"Rays"};
  AddTest(&f, TestPosition, "Computing a point from a distance");
  AddTest(&f, TestSphereIntersection, "A ray intersects a sphere at two points");
  AddTest(&f, TestSphereIntersectionTangent, "A ray intersects a sphere at one point");
  AddTest(&f, TestSphereIntersectionMisses, "A ray misses a sphere");
  AddTest(&f, TestSphereIntersectionInside, "A ray originates inside a sphere");
  AddTest(&f, TestSphereIntersectionOutside, "A ray intersects a sphere at one point");
  AddTest(&f, TestRayIntersections, "Aggregating intersections");
  AddTest(&f, TestRayHit, "The hit, when all intersections have positive t");
  AddTest(&f, TestRayHitOneNegative, "The hit, when some intersections have negative t");
  AddTest(&f, TestRayHitAllNegative, "The hit, when all intersections have negative t");
  AddTest(&f, TestRayHitOutOfOrder, "The hit is always the lowest nonnegative intersection");
  AddTest(&f, TestRayTranslation, "Translating a ray");
  AddTest(&f, TestRayScaling, "Scaling a ray");
  AddFeature(f);
}
