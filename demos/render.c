#include <math.h>
#include "../rtc.h"

int main(void) {
  Solid *floor = sphere();
  set_transform(floor, scaling(10, 0.01, 10));
  Material *m = material();
  m->specular = 0;
  set_color(m, vec(1, 0.9, 0.9));
  set_material(floor, m);

  Solid *left_wall = sphere();
  set_transform(left_wall, chain_matmul(4,
    scaling(10, 0.01, 10),
    rotation_x(M_PI/2),
    rotation_y(-M_PI/4),
    translation(0,0,5)
  ));
  set_material(left_wall, copy_material(m));

  Solid *right_wall = sphere();
  set_transform(right_wall, chain_matmul(4,
    scaling(10, 0.01, 10),
    rotation_x(M_PI/2),
    rotation_y(M_PI/4),
    translation(0,0,5)
  ));
  set_material(right_wall, copy_material(m));

  Solid *middle = sphere();
  set_transform(middle, translation(-0.5, 1, 0.5));
  m = material();
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
  add_object(w, left);
  add_object(w, middle);
  add_object(w, right);
  add_object(w, floor);
  add_object(w, left_wall);
  add_object(w, right_wall);

  Canvas *c = render(cam, w);
  canvas_to_ppm(c, stdout);
  free_world(w); free_canvas(c); free_camera(cam);
  return 0;
}
