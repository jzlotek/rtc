#include "tuple.h"
#include "../utils/funcs.h"

#pragma once

typedef struct light {
    Tuple *intensity;
    Tuple *position;
} Light;

typedef Light* LightP;

TEMPLATE_ARRAY(LightP);

typedef struct {
    Tuple *color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
} Material;


Light *point_light(Tuple *intensity, Tuple *position);
void free_point_light(Light *light);
Material *material();
void set_color(Material *m, Tuple *color);
void free_material(Material *m);
Tuple *lighting(const Material *m, const Light *l, const Tuple *point, const Tuple *eyev, const Tuple *normalv);

Light *point_light(Tuple *intensity, Tuple *position) {
    Light *l = (Light*)malloc(sizeof(Light));
    l->intensity = intensity;
    l->position = position; 
    return l;
}

void free_point_light(Light *light) {
    free(light->intensity);
    free(light->position);
    free(light);
}

Material *material() {
    Material *m = (Material*)malloc(sizeof(Material));
    m->color = vec(1, 1, 1);
    m->ambient = 0.1;
    m->diffuse = 0.9;
    m->specular = 0.9;
    m->shininess = 200.0;
    return m;
}

void set_color(Material *m, Tuple *color) {
    free(m->color);
    m->color = color;
}

void free_material(Material *m) {
    free(m->color);
    free(m);
}

Tuple *lighting(const Material *m, const Light *l, const Tuple *point, const Tuple *eyev, const Tuple *normalv) {
    Tuple *effective_color = prod(copy_tuple(m->color), l->intensity);
    Tuple *lightv = norm(sub(copy_tuple(l->position), point));
    Tuple *ambient = mult(copy_tuple(effective_color), m->ambient);

    Tuple *diffuse = vec(0, 0, 0);
    Tuple *specular = vec(0, 0, 0);
    
    float light_dot_normal = dot(lightv, normalv);
    if (light_dot_normal >= 0) {
        // copy effective color INTO diffuse
        copy_from(diffuse, effective_color);
        mult(mult(diffuse, m->diffuse), light_dot_normal);
        Tuple *reflectv = reflect(mult(lightv, -1), normalv);
        float reflect_dot_eye = dot(reflectv, eyev);

        if (reflect_dot_eye > 0) {
            float factor = pow(reflect_dot_eye, m->shininess);
            copy_from(specular, l->intensity);
            mult(mult(specular, m->specular), factor);
        }
    }

    Tuple *res = add(add(ambient, diffuse), specular);
    free(diffuse); free(specular); free(effective_color); free(lightv);
    return res;
}