#include <math.h>
#include "../base/canvas.h"
#include "../base/transforms.h"
#include "../solids/sphere.h"

int main(void) {
  Canvas *c = canvas(100, 100);
  Tuple *white = vec(1, 1, 1);
  Sphere *s = sphere();
  set_transform(s, scaling(20, 40, 20));

  int HALF_H = c->height / 2;
  int HALF_W = c->width / 2;

  for (int i = -HALF_W; i < HALF_W; i++) {
    for (int j = -HALF_H; j < HALF_H; j++) {
        Ray *r = ray(i, j, 0, 0, 0, 1);
        IntersectionArray *arr = intersect(s, r);
        Intersection h = hit(arr);

        if (h.solid != NULL) 
            write_pixel(c, i + HALF_W, j + HALF_H, white);

        free_ray(r); free_Intersection_array(arr);
    }
  }


  canvas_to_ppm(c, stdout);
  free_canvas(c);
  free_sphere(s);
  free(white);
  return 0;
}
