#include <math.h>
#include "../base/canvas.h"
#include "../base/transforms.h"
#include "../solids/sphere.h"

int main(void) {
  Canvas *c = canvas(512, 512);
  Tuple *white = vec(1, 1, 1);
  Solid *s = sphere();
  Tuple color = {1, 0.2, 1};
  copy_from(s->material->color, &color);


  Tuple *light_position = point(-10, 10, -10);
  Tuple *light_color = vec(1,1,1);
  Light *light = point_light(light_color, light_position);

  int HALF_H = c->height / 2;
  int HALF_W = c->width / 2;
  float wall_z = 10.0;
  float wall_size = 7.0;
  float pixel_size = wall_size / c->height;
  float half = wall_size / 2.0;

  Tuple *r_origin = point(0, 0, -5);

  for (int i = 0; i < c->width; i++) {
    for (int j = 0; j < c->height; j++) {
        Tuple *w_pos = point(-half + pixel_size * i, half - pixel_size * j, wall_z);
        norm(sub(w_pos, r_origin));
        Ray *r = ray(r_origin->x, r_origin->y, r_origin->z, w_pos->x, w_pos->y, w_pos->z);
        IntersectionArray *arr = intersect(s, r);
        Intersection h = hit(arr);

        if (h->solid != NULL)  {
            Tuple *point = position(r, h->t);
            Tuple *n = normal_at(h->solid, point);
            Tuple *eye = mult(r->direction, -1);
            Tuple *col = lighting(h->solid->material, light, point, eye, n, false);
            write_pixel(c, i, j, col);
            free(point); free(n); free(col);
        }

        free_ray(r); free_Intersection_array(arr); free(w_pos);
    }
  }


  canvas_to_ppm(c, stdout);
  free_canvas(c);
  free_solid(s);
  free(white);
  return 0;
}
