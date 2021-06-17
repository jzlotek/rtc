#include "test.h"

#include "test_tuple.c"
#include "test_canvas.c"
#include "test_matrix.c"
#include "test_transforms.c"
#include "test_ray.c"
#include "test_sphere.c"
#include "test_lights.c"
#include "test_materials.c"

int main() {
  TestTupleFeature();
  TestCanvasFeature();
  TestMatrixFeature();
  TestTransformsFeature();
  TestRay();
  TestSphere();
  TestLightsFeature();
  TestMaterialsFeature();
  return TestMain();
}
