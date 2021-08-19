#include "../rtc.h"

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
    m->pattern = NULL;
    return m;
}

Material *copy_material(const Material *m) {
    Material *cp = material();
    copy_from(cp->color, m->color);
    cp->ambient = m->ambient;
    cp->diffuse = m->diffuse;
    cp->specular = m->specular;
    cp->shininess = m->shininess;
    return cp;
}

void set_color(Material *m, Tuple *color) {
    free(m->color);
    m->color = color;
}

void free_material(Material *m) {
    if (m->pattern != NULL) {
        free_pattern(m->pattern);
    }
    free(m->color);
    free(m);
}

Tuple *lighting(const Material *m, const Solid *object, const Light *l, const Tuple *point, const Tuple *eyev, const Tuple *normalv, bool in_shadow) {
    Tuple *color;
    if (m->pattern == NULL) {
        color = m->color;
    } else {
        color = stripe_at_object(m->pattern, object, point);
    }
    Tuple *effective_color = prod(copy_tuple(color), l->intensity);
    Tuple *lightv = norm(sub(copy_tuple(l->position), point));
    Tuple *ambient = mult(copy_tuple(effective_color), m->ambient);

    if (in_shadow) {
        free(effective_color); free(lightv);
        return ambient;
    }

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