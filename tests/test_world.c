#include "../scene/world.h"
#include "../utils/consts.h"
#include "../base/light.h"
#include "../solids/sphere.h"
#include "test.h"

void TestWorldInitEmpty() {
  World *w = world();
  Equal(w->lights->length, 0);
  Equal(w->objects->length, 0);
  free_world(w);
}

void TestWorldInitDefault() {
  World *w = default_world();

  Tuple intensity = {1,1,1};
  Tuple pos = {-10, 10, -10, 1};
  TupleEqual(get_light(w, 0)->intensity, &intensity);
  TupleEqual(get_light(w, 0)->position, &pos);

  Solid *s = get_object(w, 0);
  Equal(s->type, SPHERE);

  Tuple color = {0.8, 1, 0.6};
  TupleEqual(s->material->color, &color);
  Equal(s->material->diffuse, 0.7);
  Equal(s->material->specular, 0.2);

  s = get_object(w, 1);
  Equal(s->type, SPHERE);
  Matrix *t = scaling(0.5, 0.5, 0.5);
  True(mateq(s->transform, t));
  free_matrix(t);

  free_world(w);
}

void TestWorldIntersection() {
  World *w = default_world();
  Ray *r = ray(0,0,-5,0,0,1);
  IntersectionArray *ia = intersect_world(w, r);
  sort_IntersectionArray(ia);

  if (Equal(ia->length, 4)) {
    Equal(get_Intersection_array(ia, 0)->t, 4);
    Equal(get_Intersection_array(ia, 1)->t, 4.5);
    Equal(get_Intersection_array(ia, 2)->t, 5.5);
    Equal(get_Intersection_array(ia, 3)->t, 6);
  }

  clean_Intersection_array(ia); free_Intersection_array(ia); free_ray(r); free_world(w);
}

void TestShadingIntersection() {
  World *w = default_world();
  Ray *r = ray(0, 0, -5, 0, 0, 1);
  Solid *shape = get_object(w, 0);
  Intersection i = intersection(4, shape);
  WorldComputation *comps = prepare_computations(i, r);

  Tuple *c = shade_hit(w, comps);
  Tuple color = {0.38066, 0.47583, 0.2855};

  free_world(w); free_ray(r); free(c); free(i); free_world_computation(comps);
}

void TestShadingIntersectionInside() {
  World *w = default_world();
  free_point_light(w->lights->data[0]);
  w->lights->data[0] = point_light(vec(1,1,1), point(0, 0.25, 0));

  Ray *r = ray(0, 0, 0, 0, 0, 1);
  Solid *shape = get_object(w, 0);
  Intersection i = intersection(0.5, shape);
  WorldComputation *comps = prepare_computations(i, r);

  Tuple *c = shade_hit(w, comps);
  Tuple color = {0.90498, 0.90498, 0.90498};

  free_world(w); free_ray(r); free(c); free(i); free_world_computation(comps);
}


void TestColorAtMiss() {
  World *w = default_world();
  Ray *r = ray(0, 0, -5, 0, 1, 0);
  Tuple *c = color_at(w, r);
  Tuple color = {0,0,0};
  TupleEqual(c, &color);

  free_world(w); free_ray(r); free(c);
}

void TestColorAtHit() {
  World *w = default_world();
  Ray *r = ray(0, 0, -5, 0, 0, 1);
  Tuple *c = color_at(w, r);
  Tuple color = {0.38066, 0.47583, 0.2855};
  TupleEqual(c, &color);

  free_world(w); free_ray(r); free(c);
}

void TestColorAtBehind() {
  World *w = default_world();
  Ray *r = ray(0, 0, 0.75, 0, 0, -1);
  Solid *outer = get_object(w, 0);
  Solid *inner = get_object(w, 1);
  outer->material->ambient = 1;
  inner->material->ambient = 1;

  Tuple *c = color_at(w, r);
  TupleEqual(c, inner->material->color);

  free_world(w); free_ray(r); free(c);
}

void TestShadowDefault() {
  World *w = default_world();
  Tuple point = {0, 10, 0, 1};
  False(is_shadowed(w, &point));
  free_world(w);
}

void TestShadowTrue() {
  World *w = default_world();
  Tuple point = {10, -10, 10, 1};
  True(is_shadowed(w, &point));
  free_world(w);
}

void TestShadowBehindFalse() {
  World *w = default_world();
  Tuple point = {-20, 20, -20, 1};
  False(is_shadowed(w, &point));
  free_world(w);
}

void TestShadowBehindPointFalse() {
  World *w = default_world();
  Tuple point = {-2, 2, -2, 1};
  False(is_shadowed(w, &point));
  free_world(w);
}

void TestShadeHit() {
  World *w = world();
  LightP_arr_add(w->lights, point_light(vec(1,1,1), point(0,0,-10)));
  Solid *s1 = sphere();
  add_object(w, s1);
  s1 = sphere();
  set_transform(s1, translation(0,0,10));
  add_object(w, s1);
  Ray *r = ray(0,0,5,0,0,1);
  Intersection i = intersection(4, s1);

  WorldComputation *comps = prepare_computations(i, r);
  Tuple *c = shade_hit(w, comps);
  Tuple exp = {0.1, 0.1, 0.1};

  TupleEqual(c, &exp);
  free_world(w); free_ray(r); free(i); free_world_computation(comps); free(c);
}

void TestWorldFeature() {
  Feature f = {"World Scene"};
  AddTest(&f, TestWorldInitEmpty, "Empty World");
  AddTest(&f, TestWorldInitDefault, "Default World");
  AddTest(&f, TestWorldIntersection, "Intersect a world with a ray");
  AddTest(&f, TestShadingIntersection, "Shading an intersection");
  AddTest(&f, TestShadingIntersectionInside, "Shading an intersection from the inside");
  AddTest(&f, TestColorAtMiss, "The color when a ray misses");
  AddTest(&f, TestColorAtHit, "The color when a ray hits");
  AddTest(&f, TestColorAtBehind, "The color with an intersection behind the ray");
  AddTest(&f, TestShadowDefault, "There is no shadow when nothing is collinear with point and light");
  AddTest(&f, TestShadowTrue, "The shadow when an object is between the point and the light");
  AddTest(&f, TestShadowBehindFalse, "There is no shadow when an object is behind the light");
  AddTest(&f, TestShadowBehindPointFalse, "There is no shadow when an object is behind the point");
  AddTest(&f, TestShadeHit, "shade_hit() is given an intersection in shadow");
  AddFeature(f);
}