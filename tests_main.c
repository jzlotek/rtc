#include "utils/test.h"

#include "base/test_tuple.c"
#include "base/test_canvas.c"
#include "base/test_matrix.c"
#include "base/test_transforms.c"

int main() {
  TestTupleFeature();
  TestCanvasFeature();
  TestMatrixFeature();
  TestTransformsFeature();
  return TestMain();
}
