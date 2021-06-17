#include "../solids/sphere.h"
#include "../base/transforms.h"
#include "test.h"


void TestSphereDefault() {
    Sphere *s = sphere();
    Matrix *ident = I();
    True(mateq(s->transform, ident));

    free(ident); free_sphere(s);
}

void TestSphereTransform() {
    Sphere *s = sphere();
    Matrix *exp = translation(2, 3, 4);
    set_transform(s, translation(2, 3, 4));
    True(mateq(s->transform, exp));

    free_sphere(s); free(exp);
}

void TestSphereIntersect() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Sphere *s = sphere();
    set_transform(s, scaling(2, 2, 2));
    IntersectionArray *arr = intersect(s, r);
    if (Equal(arr->length, 2)) {
        Equal(get_Intersection_array(arr, 0).t, 3);
        Equal(get_Intersection_array(arr, 1).t, 7);
    }

    free_ray(r); free_Intersection_array(arr); free_sphere(s);
}

void TestSphereIntersectMiss() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Sphere *s = sphere();
    set_transform(s, translation(5, 0, 0));
    IntersectionArray *arr = intersect(s, r);
    Equal(arr->length, 0);

    free_ray(r); free_Intersection_array(arr); free_sphere(s);
}

void TestSphereNormals() {
    Sphere *s = sphere();

    Tuple norms[4] = {
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 1, 1},
        {sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3, 1},
    };
    Tuple exp[4] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3},
    };

    for (unsigned int i = 0; i < 1; i++) {
        Tuple *res = normal_at(s, &norms[i]);
        TupleEqual(res, &exp[i]);
        Equal(magnitude(res), 1);
        free(res);
    }

    free_sphere(s);
}

void TestSphereNormalsTranslate() {
    Sphere *s = sphere();
    set_transform(s, translation(0, 1, 0));

    Tuple *p = point(0, 1.70711, -0.70711);
    Tuple *res = normal_at(s, p);
    Tuple *exp = vec(0, 0.70711, -0.70711);

    TupleEqual(res, exp);
    Equal(magnitude(res), 1);
    free(res); free_sphere(s); free(p); free(exp);
}

void TestSphereNormalsScaleRot() {
    Sphere *s = sphere();
    set_transform(s, chain_matmul(2,
        rotation_z(M_PI / 5),
        scaling(1.0, 0.5, 1.0)
    ));

    Tuple *p = point(0, sqrt(2)/2, -sqrt(2)/2);
    Tuple *res = normal_at(s, p);
    Tuple *exp = vec(0, 0.97014, -0.24254);

    TupleEqual(res, exp);
    free(res); free_sphere(s); free(p); free(exp);
}

void TestSphereMaterialDefault() {
    Sphere *s = sphere();
    Material *m = material();
    m->ambient = 1.0;
    set_material(s, m);
    Equal(s->material->ambient, 1.0);
    Equal(s->material->diffuse, 0.9);
    Equal(s->material->specular, 0.9);
    Equal(s->material->shininess, 200.0);
    free_sphere(s);
}

void  TestSphere() {
  Feature f = {"Spheres"};
  AddTest(&f, TestSphereDefault, "A sphere's default transformation");
  AddTest(&f, TestSphereTransform, "Changing the sphere's transform");
  AddTest(&f, TestSphereIntersect, "Intersecting a scaled sphere with a ray");
  AddTest(&f, TestSphereIntersectMiss, "Intersecting a translated sphere with a ray");
  AddTest(&f, TestSphereNormals, "The normal on a sphere at points");
  AddTest(&f, TestSphereNormalsTranslate, "The normal on a translated sphere at points");
  AddTest(&f, TestSphereNormalsScaleRot, "The normal on a transformed sphere at points");
  AddTest(&f, TestSphereMaterialDefault, "Sphere Material");
  AddFeature(f);
}