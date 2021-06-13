#include <math.h>
#include "../base/canvas.h"
#include "../base/transforms.h"

int main(void) {
  Canvas *c = canvas(100, 100);
  Tuple *c1 = vec(1, 1, 1);

  const int HALF_H = 50;
  const int HALF_W = 50;

  for (unsigned int i = 0; i < 12; i++) {
    Tuple *hand = vec(0, 1, 0);
    Matrix *t = chain_matmul(3,
        scaling(0, 25, 0),
        translation(0, 50, 0),
        rotation_z(M_PI / 6 * i)
    );
    apply(hand, t);
    write_pixel(c, hand->x + HALF_W, hand->y + HALF_H, c1);
    free(t); free(hand);
  }


  canvas_to_ppm(c, stdout);
  free_canvas(c);
  free(c1);
  return 0;
}
