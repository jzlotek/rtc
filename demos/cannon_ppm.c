#include <stdio.h>
#include "../base/canvas.h"

void tick(tuple *p_pos, tuple *p_vel, tuple *env_g, tuple *env_wind) {
  add(p_pos, p_vel);
  add(p_vel, env_g);
  add(p_vel, env_wind);
}

int main(void) {
  Canvas *c = canvas(900, 550);
  tuple *c1 = vec(1, 0.8, 0.6);

  tuple *start = vec(0, 1.0, 0);
  tuple *vel = mult(norm(vec(1, 1.8, 0)), 11.25);
  tuple *G = vec(0, -0.1, 0);
  tuple *wind = vec(-0.01, 0.0, 0.0);


  write_pixel(c, start->x, c->height - start->y, c1);
  while (start->y > 0) {
    tick(start, vel, G, wind);
    write_pixel(c, start->x, c->height - start->y, c1);
  }

  canvas_to_ppm(c, stdout);
  free_canvas(c);
  free(c1);
  free(start); free(vel); free(G); free(wind);
  return 0;
}
