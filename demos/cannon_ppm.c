#include <stdio.h>
#include "../rtc.h"

void tick(Tuple *p_pos, Tuple *p_vel, Tuple *env_g, Tuple *env_wind) {
  add(p_pos, p_vel);
  add(p_vel, env_g);
  add(p_vel, env_wind);
}

int main(void) {
  Canvas *c = canvas(900, 550);
  Tuple *c1 = vec(1, 0.8, 0.6);

  Tuple *start = vec(0, 1.0, 0);
  Tuple *vel = mult(norm(vec(1, 1.8, 0)), 11.25);
  Tuple *G = vec(0, -0.1, 0);
  Tuple *wind = vec(-0.01, 0.0, 0.0);


  write_pixel(c, start->vals[0], c->height - start->vals[1], c1);
  while (start->vals[1] > 0) {
    tick(start, vel, G, wind);
    write_pixel(c, start->vals[0], c->height - start->vals[1], c1);
  }

  canvas_to_ppm(c, stdout);
  free_canvas(c);
  free(c1);
  free(start); free(vel); free(G); free(wind);
  return 0;
}
