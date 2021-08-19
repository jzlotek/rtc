#include "../rtc.h"
#include "test.h"

void TestMaterialDefault() {
    Material *m = material();
    Tuple color = {1,1,1};
    TupleEqual(m->color, &color);
    Equal(m->ambient, 0.1);
    Equal(m->diffuse, 0.9);
    Equal(m->specular, 0.9);
    Equal(m->shininess, 200.0);
    free_material(m);
}

void TestMaterialLighting() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, 0, -1);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,0,-10));
  Solid *s = sphere();
  Tuple *res = lighting(m, s, l, pos, eyev, normalv, false);
  Tuple exp = {1.9,1.9,1.9};
  TupleEqual(res, &exp);
  free_solid(s);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingAngle() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, sqrt(2)/2, -sqrt(2)/2);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,0,-10));
  Solid *s = sphere();
  Tuple *res = lighting(m, s, l, pos, eyev, normalv, false);
  Tuple exp = {1.0,1.0,1.0};
  TupleEqual(res, &exp);
  free_solid(s);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingAngleOpposite() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0,0,-1);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,10,-10));
  Solid *s = sphere();
  Tuple *res = lighting(m, s, l, pos, eyev, normalv, false);
  Tuple exp = {0.7364, 0.7364, 0.7364};
  TupleEqual(res, &exp);
  free_solid(s);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingReflectionVec() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, -sqrt(2)/2, -sqrt(2)/2);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,10,-10));
  Solid *s = sphere();
  Tuple *res = lighting(m, s, l, pos, eyev, normalv, false);
  Tuple exp = {1.6364, 1.6364, 1.6364};
  TupleEqual(res, &exp);
  free_solid(s);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingBehindSurface() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, 0, -1);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,0,10));
  Solid *s = sphere();
  Tuple *res = lighting(m, s, l, pos, eyev, normalv, false);
  Tuple exp = {0.1, 0.1, 0.1};
  TupleEqual(res, &exp);
  free_solid(s);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialShadow() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, 0, -1);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,0,-10));
  Solid *s = sphere();
  Tuple *res = lighting(m, s, l, pos, eyev, normalv, true);
  Tuple exp = {0.1, 0.1, 0.1};
  TupleEqual(res, &exp);
  free_solid(s);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialPattern() {
  Light *l = point_light(vec(1,1,1), point(0,0,-10));
  Tuple *eyev = vec(0, 0, -1);
  Tuple *normalv = vec(0, 0, -1);

  Material *m = material();
  m->ambient = 1;
  m->diffuse = 0;
  m->specular = 0;
  m->pattern = stripe_pattern(vec(1,1,1), vec(0,0,0));

  Tuple *pos1 = point(0.9,0,0);
  Tuple *pos2 = point(1.1,0,0);

  Solid *s = sphere();
  Tuple *c1 = lighting(m, s, l, pos1, eyev, normalv, true);
  Tuple *c2 = lighting(m, s, l, pos2, eyev, normalv, true);
  Tuple c1_exp = {0.1, 0.1, 0.1};
  Tuple c2_exp = {0.0, 0.0, 0.0};
  free_solid(s);
  TupleEqual(c1, &c1_exp);
  TupleEqual(c2, &c2_exp);
  free(eyev); free(normalv); free_point_light(l); free(pos1); free(pos2); free_material(m);
}

void TestMaterialsFeature() {
  Feature f = {"Materials"};
  AddTest(&f, TestMaterialDefault, "Default Material");
  AddTest(&f, TestMaterialLighting, "Lighting with the eye between the light and the surface");
  AddTest(&f, TestMaterialLightingAngle, "Lighting with the eye between light and surface, eye offset 45°");
  AddTest(&f, TestMaterialLightingAngleOpposite, "Lighting with eye opposite surface, light offset 45°");
  AddTest(&f, TestMaterialLightingReflectionVec, "Lighting with eye in the path of the reflection vector");
  AddTest(&f, TestMaterialLightingBehindSurface, "Lighting with the light behind the surface");
  AddTest(&f, TestMaterialShadow, "Lighting with the surface in shadow");
  AddTest(&f, TestMaterialPattern, "Lighting with a pattern applied");
  AddFeature(f);
}
