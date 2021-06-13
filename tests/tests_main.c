#include "test.h"

#include "test_tuple.c"
#include "test_canvas.c"
#include "test_matrix.c"
#include "test_transforms.c"
#include "test_ray.c"
#include "test_sphere.c"

int main() {
  TestTupleFeature();
  TestCanvasFeature();
  TestMatrixFeature();
  TestTransformsFeature();
  TestRay();
  TestSphere();
  return TestMain();
}
