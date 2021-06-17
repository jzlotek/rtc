#include "../base/light.h"
#include "../utils/consts.h"
#include "test.h"

void TestMaterialInitDefault() {
    Tuple *intensity = vec(1,1,1), *position = point(0,0,0);
    Light *l = point_light(intensity, position);
    TupleEqual(l->intensity, intensity);
    TupleEqual(l->position, position);
    free_point_light(l);
}

void TestLightsFeature() {
  Feature f = {"Lights"};
  AddTest(&f, TestMaterialInitDefault, "Reflecting Vectors");
  AddFeature(f);
}