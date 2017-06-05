#pragma once

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    enum {
        Circle,
        Rectangle,
        Square,
    } t;

    Point p;

    union {
        struct {
            double r;
        } circle;

        struct {
            double l;
        } square;

        struct {
            double h;
            double w;
        } rectangle;
    } v;
} Shape;

double area(Shape*);
bool inside(Shape*, Point*);
void show_point(Point*, FILE*);
void show_shape(Shape*, FILE*);
