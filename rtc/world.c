#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../rtc.h"

void clean_Solid_array(SolidPArray *ar) {
    for (unsigned int i = 0; i < ar->length; i++) {
        free_solid(ar->data[i]);
    }
}

World *world() {
    World *w = (World*)malloc(sizeof(World));
    w->lights = LightP_array();
    w->objects = SolidP_array();
    return w;
}

void add_object(World *w, SolidP o) {
    SolidP_arr_add(w->objects, o);
}

void add_light(World *w, Light *l) {
    LightP_arr_add(w->lights, l);
}

World *default_world() {
    World *w = world();
    LightP l = point_light(vec(1,1,1), point(-10, 10, -10));
    LightP_arr_add(w->lights, l);

    Solid *s = sphere();
    Material *m = material();
    set_color(m, vec(0.8, 1, 0.6));
    m->diffuse = 0.7;
    m->specular = 0.2;

    set_material(s, m);
    add_object(w, s);

    s = sphere();
    set_transform(s, scaling(0.5, 0.5, 0.5));
    add_object(w, s);

    return w;
}

Solid *get_object(World *w, unsigned int i) {
    return get_SolidP_array(w->objects, i);
}

Light *get_light(World *w, unsigned int i) {
    return get_LightP_array(w->lights, i);
}

void free_world(World *w) {
    clean_Solid_array(w->objects);
    free_SolidP_array(w->objects);

    for (unsigned int i = 0; i < w->lights->length; i++) {
        free_point_light(get_LightP_array(w->lights, i));
    }
    free_LightP_array(w->lights);
    free(w);
}

IntersectionArray *intersect_world(World *w, Ray *r) {
    IntersectionArray *ia = Intersection_array();

    for (unsigned int io = 0; io < w->objects->length; io++) {
        Solid *s = get_SolidP_array(w->objects, io);

        IntersectionArray *intersectedWith = intersect(s, r);
        for (unsigned int i = 0; i < intersectedWith->length; i++) {
            Intersection_arr_add(ia, get_Intersection_array(intersectedWith, i));
        }
        free_Intersection_array(intersectedWith);
    }

    return ia;
}

WorldComputation *world_computation(Solid* o, float t, Tuple *point, Tuple *eyev, Tuple *normalv) {
    WorldComputation *wc = (WorldComputation*)malloc(sizeof(WorldComputation));
    wc->t = t;
    wc->obj = o;
    wc->point = point;
    wc->eyev = eyev;
    wc->normalv = normalv;
    wc->inside = false;
    wc->over_point = add(mult(copy_tuple(wc->normalv), EPSILON), wc->point);
    return wc;
}

void free_world_computation(WorldComputation *wc) {
    free(wc->normalv);
    free(wc->point);
    free(wc->eyev);
    free(wc->over_point);
    free(wc);
}

WorldComputation *prepare_computations(Intersection i, Ray *r) {
    Tuple *pt = position(r, i->t);
    WorldComputation *wc = world_computation(
        i->solid,
        i->t,
        pt,
        mult(copy_tuple(r->direction), -1),
        normal_at(i->solid, pt)
    );
    if (dot(wc->normalv, wc->eyev) < 0) {
        wc->inside = true;
        mult(wc->normalv, -1);
    }
    return wc;
}

Tuple *shade_hit(World *w, WorldComputation *wc) {
    Tuple *final_color = vec(0,0,0);
    for (unsigned int i = 0; i < w->lights->length; i++) {
        Tuple *c = lighting(
            wc->obj->material,
            wc->obj,
            get_LightP_array(w->lights, i),
            wc->point,
            wc->eyev,
            wc->normalv,
            is_shadowed(w, wc->over_point)
        );
        add(final_color, c);
        free(c);
    }
    return final_color;
}

Tuple *color_at(World *w, Ray *r) {
    IntersectionArray *ia = intersect_world(w, r);
    Intersection i = hit(ia);
    if (i->solid == NULL) {
        clean_Intersection_array(ia); free_Intersection_array(ia); free(i);
        return vec(0,0,0);
    }

    WorldComputation *comps = prepare_computations(i, r);
    Tuple *c = shade_hit(w, comps);
    clean_Intersection_array(ia); free_Intersection_array(ia); free(i); free_world_computation(comps);
    return c;
}

bool is_shadowed(World *w, Tuple *point) {
    for (unsigned int i = 0; i < w->lights->length; i++) {
        Light *l = get_LightP_array(w->lights, i);
        Tuple *direction = sub(copy_tuple(l->position), point);
        float distance = magnitude(direction);
        norm(direction);
        Ray *r = ray(point->vals[0], point->vals[1], point->vals[2], direction->vals[0], direction->vals[1], direction->vals[2]);
        IntersectionArray *ia = intersect_world(w, r);
        Intersection h = hit(ia);
        clean_Intersection_array(ia); free_Intersection_array(ia); free(direction); free_ray(r);
        if (h->solid != NULL && h->t < distance) {
            free(h);
            return true;
        } else {
            free(h);
            return false;
        }
    }
    return false;
}
