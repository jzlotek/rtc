#include "../scene/world.h"
#include "../utils/consts.h"

#include "test.h"

void TestPrepareComputationDefault() {
  Ray *r = ray(0,0,-5,0,0,1);
  Solid *s = sphere();
  Intersection i = intersection(4, s);
  WorldComputation *wc = prepare_computations(i, r);

  Equal(wc->t, i->t);
  Equal(*(float*)wc->obj, *(float*)s);

  free_ray(r); free_solid(s); free(i); free_world_computation(wc);
}

void TestPrepareComputationInside() {
  Ray *r = ray(0,0,0,0,0,1);
  Solid *s = sphere();
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

  free_ray(r); free_solid(s); free(i); free_world_computation(wc);
}

void TestIntersectionHitOffset() {
  Ray *r = ray(0,0,-5, 0,0,1);
  Solid *s = sphere();
  set_transform(s, translation(0,0,1));
  Intersection i = intersection(5, s);
  WorldComputation *comps = prepare_computations(i, r);
  True(comps->over_point->z < -EPSILON/2);
  True(comps->point->z > comps->over_point->z);
  free_ray(r); free(i); free_solid(s); free_world_computation(comps);
}

void TestIntersectionsFeature() {
  Feature f = {"Intersections"};
  AddTest(&f, TestPrepareComputationDefault, "Precomputing the state of an intersection");
  AddTest(&f, TestPrepareComputationInside, "The hit, when an intersection occurs on the inside");
  AddTest(&f, TestIntersectionHitOffset, "The hit should offset the point");
  AddFeature(f);
}