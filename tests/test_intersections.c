#include "../scene/world.h"
#include "../utils/consts.h"

#include "test.h"

void TestPrepareComputationDefault() {
  Ray *r = ray(0,0,-5,0,0,1);
  Sphere *s = sphere();
  Intersection i = intersection(4, s);
  WorldComputation *wc = prepare_computations(i, r);

  Equal(wc->t, i->t);
  Equal(*(float*)wc->obj, *(float*)s);

  free_ray(r); free_sphere(s); free(i); free_world_computation(wc);
}

void TestPrepareComputationInside() {
  Ray *r = ray(0,0,0,0,0,1);
  Sphere *s = sphere();
  Intersection i = intersection(1, s);
  WorldComputation *wc = prepare_computations(i, r);

  Equal(wc->t, i->t);
  Equal(*(float*)wc->obj, *(float*)s);

  Tuple eyev = {0,0,-1};
  Tuple point = {0,0,1,1};
  Tuple normalv = {0,0,-1};

  TupleEqual(wc->eyev, &eyev);
  TupleEqual(wc->point, &point);
  TupleEqual(wc->normalv, &normalv);
  True(wc->inside);

  free_ray(r); free_sphere(s); free(i); free_world_computation(wc);
}

void TestIntersectionsFeature() {
  Feature f = {"Intersections"};
  AddTest(&f, TestPrepareComputationDefault, "Precomputing the state of an intersection");
  AddTest(&f, TestPrepareComputationInside, "The hit, when an intersection occurs on the inside");
  AddFeature(f);
}