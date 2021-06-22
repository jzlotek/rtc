#include "../scene/camera.h"
#include "../utils/consts.h"
#include "test.h"

#include <math.h>

#pragma once


void TestCameraCreation() {
    Camera *c = camera(160, 120, M_PI/2);
    Matrix *i = I();
    Equal(c->hsize, 160);
    Equal(c->vsize, 120);
    Equal(c->field_of_view, M_PI/2);
    True(mateq(c->transform, i));
    free_camera(c); free_matrix(i);
}

void TestCameraPixelSizeH() {
    Camera *c = camera(200, 125, M_PI/2);
    Equal(c->pixel_size, 0.01);
    free_camera(c);
}

void TestCameraPixelSizeV() {
    Camera *c = camera(125, 200, M_PI/2);
    Equal(c->pixel_size, 0.01);
    free_camera(c);
}

void TestCameraRayCenter() {
    Camera *c = camera(201, 101, M_PI/2);
    Ray *r = ray_for_pixel(c, 100, 50);

    Tuple origin = {0,0,0,1};
    Tuple direction = {0, 0, -1};
    TupleEqual(r->origin, &origin);
    TupleEqual(r->direction, &direction);
    free_camera(c); free_ray(r);
}

void TestCameraRayCorner() {
    Camera *c = camera(201, 101, M_PI/2);
    Ray *r = ray_for_pixel(c, 0, 0);

    Tuple origin = {0,0,0,1};
    Tuple direction = {0.66519, 0.33259, -0.66851};
    TupleEqual(r->origin, &origin);
    TupleEqual(r->direction, &direction);
    free_camera(c); free_ray(r);
}

void TestCameraRayTransform() {
    Camera *c = camera(201, 101, M_PI/2);
    set_camera_transform(c, chain_matmul(2,
     translation(0, -2, 5),
     rotation_y(M_PI/4)
     ));
    Ray *r = ray_for_pixel(c, 100, 50);

    Tuple origin = {0,2,-5,1};
    Tuple direction = {sqrt(2)/2, 0, -sqrt(2)/2};
    TupleEqual(r->origin, &origin);
    TupleEqual(r->direction, &direction);
    free_camera(c); free_ray(r);
}


void TestRenderCamera() {
    World *w = default_world();
    Camera *c = camera(11, 11, M_PI/2);
    set_camera_transform(c, view_transform(point(0, 0, -5), point(0, 0, 0), vec(0, 1, 0)));
    Canvas *image = render(c, w);
    Tuple color = {0.38066, 0.47583, 0.2855};

    TupleEqual(pixel_at(image, 5, 5), &color);
    free_world(w); free_camera(c); free_canvas(image);
}

void TestCameraFeature() {
  Feature f = {"Camera"};
  AddTest(&f, TestCameraCreation, "Constructing a camera");
  AddTest(&f, TestCameraPixelSizeH, "The pixel size for a horizontal canvas");
  AddTest(&f, TestCameraPixelSizeV, "The pixel size for a vertical canvas");
  AddTest(&f, TestCameraRayCenter, "Constructing a ray through the center of the canvas");
  AddTest(&f, TestCameraRayCorner, "Constructing a ray through a corner of the canvas");
  AddTest(&f, TestCameraRayTransform, "Constructing a ray when the camera is transformed");
  AddTest(&f, TestRenderCamera, "Rendering a world with a camera");
  AddFeature(f);
}
