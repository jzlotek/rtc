#include "../base/light.h"
#include "../solids/sphere.h"
#include "../utils/consts.h"
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
  Tuple *res = lighting(m, l, pos, eyev, normalv, false);
  Tuple exp = {1.9,1.9,1.9};
  TupleEqual(res, &exp);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingAngle() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, sqrt(2)/2, -sqrt(2)/2);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,0,-10));
  Tuple *res = lighting(m, l, pos, eyev, normalv, false);
  Tuple exp = {1.0,1.0,1.0};
  TupleEqual(res, &exp);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingAngleOpposite() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0,0,-1);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,10,-10));
  Tuple *res = lighting(m, l, pos, eyev, normalv, false);
  Tuple exp = {0.7364, 0.7364, 0.7364};
  TupleEqual(res, &exp);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingReflectionVec() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, -sqrt(2)/2, -sqrt(2)/2);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,10,-10));
  Tuple *res = lighting(m, l, pos, eyev, normalv, false);
  Tuple exp = {1.6364, 1.6364, 1.6364};
  TupleEqual(res, &exp);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialLightingBehindSurface() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, 0, -1);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,0,10));
  Tuple *res = lighting(m, l, pos, eyev, normalv, false);
  Tuple exp = {0.1, 0.1, 0.1};
  TupleEqual(res, &exp);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
}

void TestMaterialShadow() {
  Material *m = material();
  Tuple *pos = point(0,0,0);
  Tuple *eyev = vec(0, 0, -1);
  Tuple *normalv = vec(0, 0, -1);
  Light *l = point_light(vec(1,1,1), point(0,0,-10));
  Tuple *res = lighting(m, l, pos, eyev, normalv, true);
  Tuple exp = {0.1, 0.1, 0.1};
  TupleEqual(res, &exp);
  free(eyev); free(normalv); free_point_light(l); free(pos); free_material(m); free(res);
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
  AddFeature(f);
}