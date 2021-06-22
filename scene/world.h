#include <stdlib.h>
#include <stdio.h>

#include "../base/light.h"
#include "../base/transforms.h"
#include "../utils/funcs.h"
#include "../utils/consts.h"
#include "../solids/sphere.h"


#pragma once

typedef struct object {
    ObjectType type;
    void *data;
    void (*free_func)(void*);
    IntersectionArray (*intersect)(void*, Ray*);
} __object;

typedef __object* Object;
TEMPLATE_ARRAY(Object);

typedef struct world {
    LightPArray *lights;
    ObjectArray *objects;
} World;

typedef struct computations {
    Object obj;
    float t;
    bool inside;
    Tuple *point;
    Tuple *eyev;
    Tuple *normalv;
} WorldComputation;

Object object(ObjectType type, void *data, void (*free_func)(void*));
void clean_Object_array(ObjectArray *ar);
World *world();
World *default_world();
void add_object(World *w, Object o);
void free_world(World *w);
IntersectionArray *intersect_world(World *w, Ray *r);
WorldComputation *world_computation(Object o, float t, Tuple *point, Tuple *eyev, Tuple *normalv);
void free_world_computation(WorldComputation *wc);
WorldComputation *prepare_computations(Intersection i, Ray *r);
Tuple *shade_hit(World *w, WorldComputation *wc);
Tuple *color_at(World *w, Ray *r);

Object object(ObjectType type, void *data, void (*free_func)(void*)) {
    Object o = (Object)malloc(sizeof(__object));
    o->type = type;
    o->data = data;
    o->free_func = free_func;
    return o;
}

void clean_Object_array(ObjectArray *ar) {
    for (unsigned int i = 0; i < ar->length; i++) {
        free(ar->data[i]);
    }
}

World *world() {
    World *w = (World*)malloc(sizeof(World));
    w->lights = LightP_array();
    w->objects = Object_array();
    return w;
}

void add_object(World *w, Object o) {
    Object_arr_add(w->objects, o);
}

World *default_world() {
    World *w = world();
    LightP l = point_light(vec(1,1,1), point(-10, 10, -10));
    LightP_arr_add(w->lights, l);

    Sphere *s = sphere();
    Material *m = material();
    set_color(m, vec(0.8, 1, 0.6));
    m->diffuse = 0.7;
    m->specular = 0.2;

    set_material(s, m);
    add_object(w, object(SPHERE, s, (void (*)(void*))free_sphere));
    
    s = sphere();
    set_transform(s, scaling(0.5, 0.5, 0.5));
    add_object(w, object(SPHERE, s, (void (*)(void*))free_sphere));

    return w;
}

void free_world(World *w) {
    for (unsigned int i = 0; i < w->objects->length; i++) {
        void (*free_func)(void*) = get_Object_array(w->objects, i)->free_func;
        free_func(get_Object_array(w->objects, i)->data);
        free(get_Object_array(w->objects, i));
    }
    free_Object_array(w->objects);

    for (unsigned int i = 0; i < w->lights->length; i++) {
        free_point_light(get_LightP_array(w->lights, i));
    }
    free_LightP_array(w->lights);
    free(w);
}

IntersectionArray *intersect_world(World *w, Ray *r) {
    IntersectionArray *ia = Intersection_array();

    for (unsigned int io = 0; io < w->objects->length; io++) {
        Sphere *s = (Sphere*)get_Object_array(w->objects, io)->data;

        IntersectionArray *intersectedWith = intersect(s, r);
        for (unsigned int i = 0; i < intersectedWith->length; i++) {
            Intersection_arr_add(ia, get_Intersection_array(intersectedWith, i));
        }
        free_Intersection_array(intersectedWith);
    }

    return ia;
}

WorldComputation *world_computation(Object o, float t, Tuple *point, Tuple *eyev, Tuple *normalv) {
    WorldComputation *wc = (WorldComputation*)malloc(sizeof(WorldComputation));
    wc->t = t;
    wc->obj = o;
    wc->point = point;
    wc->eyev = eyev;
    wc->normalv = normalv;
    wc->inside = false;
    return wc;
}

void free_world_computation(WorldComputation *wc) {
    free(wc->normalv);
    free(wc->point);
    free(wc->eyev);
    free(wc);
}

WorldComputation *prepare_computations(Intersection i, Ray *r) {
    Tuple *pt = position(r, i->t);
    WorldComputation *wc = world_computation(
        i->solid,
        i->t,
        pt,
        mult(copy_tuple(r->direction), -1),
        normal_at((Sphere*)i->solid, pt)
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
            ((Sphere*)wc->obj)->material,
            get_LightP_array(w->lights, i),
            wc->point,
            wc->eyev,
            wc->normalv
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
    // printf("\n");
    // print_tuple(comps->point);
    // printf("\n");
    // print_tuple(comps->eyev);
    // printf("\n");
    // print_tuple(comps->normalv);
    // printf("\n");
    Tuple *c = shade_hit(w, comps);
    // print_tuple(c);
    // printf("\n");
    clean_Intersection_array(ia); free_Intersection_array(ia); free(i);
    return c;
}