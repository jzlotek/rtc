#include "test.h"

#include "test_tuple.c"
#include "test_canvas.c"
#include "test_matrix.c"
#include "test_transforms.c"
#include "test_ray.c"
#include "test_sphere.c"
#include "test_lights.c"
#include "test_materials.c"
#include "test_world.c"
#include "test_intersections.c"
#include "test_camera.c"
#include "test_solid.c"
#include "test_plane.c"
#include "test_patterns.c"

int main() {
  TestTupleFeature();
  TestCanvasFeature();
  TestMatrixFeature();
  TestTransformsFeature();
  TestRay();
  TestSphere();
  TestLightsFeature();
  TestMaterialsFeature();
  TestWorldFeature();
  TestIntersectionsFeature();
  TestCameraFeature();
  TestSolidFeature();
  TestPlaneFeature();
  TestPatternsFeature();
  return TestMain();
}
