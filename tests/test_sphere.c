#include "../rtc.h"

#include "test.h"


void TestSphereDefault() {
    Solid *s = sphere();
    Matrix *ident = I();
    True(mateq(s->transform, ident));

    free_matrix(ident); free_solid(s);
}

void TestSphereTransform() {
    Solid *s = sphere();
    Matrix *exp = translation(2, 3, 4);
    set_transform(s, translation(2, 3, 4));
    True(mateq(s->transform, exp));

    free_solid(s); free_matrix(exp);
}

void TestSphereIntersect() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Solid *s = sphere();
    set_transform(s, scaling(2, 2, 2));
    IntersectionArray *arr = intersect(s, r);
    if (Equal(arr->length, 2)) {
        Equal(get_Intersection_array(arr, 0)->t, 3);
        Equal(get_Intersection_array(arr, 1)->t, 7);
    }
    clean_Intersection_array(arr); free_ray(r); free_Intersection_array(arr); free_solid(s);
}

void TestSphereIntersectMiss() {
    Ray *r = ray(0, 0, -5, 0, 0, 1);
    Solid *s = sphere();
    set_transform(s, translation(5, 0, 0));
    IntersectionArray *arr = intersect(s, r);
    Equal(arr->length, 0);

    clean_Intersection_array(arr); free_ray(r); free_Intersection_array(arr); free_solid(s);
}

void TestSphereNormals() {
    Solid *s = sphere();

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

    free_solid(s);
}

void TestSphereNormalsTranslate() {
    Solid *s = sphere();
    set_transform(s, translation(0, 1, 0));

    Tuple *p = point(0, 1.70711, -0.70711);
    Tuple *res = normal_at(s, p);
    Tuple *exp = vec(0, 0.70711, -0.70711);

    TupleEqual(res, exp);
    Equal(magnitude(res), 1);
    free(res); free_solid(s); free(p); free(exp);
}

void TestSphereNormalsScaleRot() {
    Solid *s = sphere();
    set_transform(s, chain_matmul(2,
        rotation_z(M_PI / 5),
        scaling(1.0, 0.5, 1.0)
    ));

    Tuple *p = point(0, sqrt(2)/2, -sqrt(2)/2);
    Tuple *res = normal_at(s, p);
    Tuple *exp = vec(0, 0.97014, -0.24254);

    TupleEqual(res, exp);
    free(res); free_solid(s); free(p); free(exp);
}

void TestSphereMaterialDefault() {
    Solid *s = sphere();
    Material *m = material();
    m->ambient = 1.0;
    set_material(s, m);
    Equal(s->material->ambient, 1.0);
    Equal(s->material->diffuse, 0.9);
    Equal(s->material->specular, 0.9);
    Equal(s->material->shininess, 200.0);
    free_solid(s);
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
