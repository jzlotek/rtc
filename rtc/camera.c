#include <stdlib.h>

#include "../rtc.h"

Camera *camera(unsigned int hsize, unsigned int vsize, float field_of_view) {
    Camera *c = (Camera*)malloc(sizeof(Camera));
    c->hsize = hsize;
    c->vsize = vsize;
    c->field_of_view = field_of_view;
    c->transform = I();

    float half_view = tan(field_of_view / 2);
    float aspect = (float)hsize / (float)vsize;

    if (aspect >= 1) {
        c->half_width = half_view;
        c->half_height = half_view / aspect;
    } else {
        c->half_width = half_view * aspect;
        c->half_height = half_view;
    }
    c->pixel_size = (c->half_width * 2) / (float)hsize;

    return c;
}

void free_camera(Camera *c) {
    free_matrix(c->transform);
    free(c);
}

Camera *set_camera_transform(Camera *c, Matrix *t) {
    free_matrix(c->transform);
    c->transform = t;
    return c;
}

Ray *ray_for_pixel(Camera *c, int x, int y) {
    float xoffset = (x + 0.5) * c->pixel_size;
    float yoffset = (y + 0.5) * c->pixel_size;
    float world_x = c->half_width - xoffset;
    float world_y = c->half_height - yoffset;

    Matrix *t_inv  = inverse(c->transform);
    Tuple *pixel = apply(point(world_x, world_y, -1), t_inv);
    Tuple *origin = apply(point(0, 0, 0), t_inv);
    Tuple *direction = norm(sub(pixel, origin));
    Ray *r = ray(
            origin->vals[0],
            origin->vals[1],
            origin->vals[2],
            direction->vals[0],
            direction->vals[1],
            direction->vals[2]
    );
    free(direction); free(origin); free_matrix(t_inv);
    return r;
}

Canvas *render(Camera *c, World *w) {
    Canvas *cv = canvas(c->hsize, c->vsize);
    for (int j = 0; j < c->vsize; j++) {
        for (int i = 0; i < c->hsize; i++) {
            Ray *r = ray_for_pixel(c, i, j);
            Tuple *color = color_at(w, r);
            write_pixel(cv, i, j, color);
            free(color); free_ray(r);
        }
    }
    return cv;
}
