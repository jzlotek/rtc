#include <math.h>
#include "../scene/camera.h"
#include "../scene/world.h"
#include "../solids/plane.h"
#include "../solids/sphere.h"

int main(void) {
  Solid *p = plane();


  Solid *middle = sphere();
  set_transform(middle, translation(-0.5, 1, 0.5));
  Material *m = material();
  set_color(m, vec(0.1, 1, 0.5));
  m->diffuse = 0.7;
  m->specular = 0.3;
  set_material(middle, m);

  Solid *right = sphere();
  set_transform(right, chain_matmul(2, scaling(0.5, 0.5, 0.5), translation(1.5, 0.5, -0.5)));
  m = material();
  set_color(m, vec(0.5, 1, 0.1));
  m->diffuse = 0.7;
  m->specular = 0.3;
  set_material(right, m);

  Solid *left = sphere();
  set_transform(left, chain_matmul(2, scaling(0.33, 0.33, 0.33), translation(-1.5, 0.33, -0.75)));
  m = material();
  set_color(m, vec(1, 0.8, 0.1));
  m->diffuse = 0.7;
  m->specular = 0.3;
  set_material(left, m);

  Camera *cam = set_camera_transform(
    camera(1000, 500, M_PI/3),
    view_transform(point(0, 1.5, -5), point(0, 1, 0), vec(0, 1, 0))
  );


  World *w = world();
  add_light(w, point_light(vec(1,1,1), point(-10, 10, -10)));
  add_object(w, p);
  add_object(w, left);
  add_object(w, middle);
  add_object(w, right);

  Canvas *c = render(cam, w);
  canvas_to_ppm(c, stdout);
  free_world(w); free_canvas(c); free_camera(cam);
  return 0;
}
