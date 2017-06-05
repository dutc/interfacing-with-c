#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "shape.h"

double area(Shape* s) {
    switch (s->t) {
        case Circle:    return M_PI * pow(s->v.circle.r, 2);
        case Rectangle: return s->v.rectangle.h * s->v.rectangle.w;
        case Square:    return pow(s->v.square.l, 2);
        default:
            fprintf(stderr, "unknown shape type: %d\n", s->t);
            exit(EXIT_FAILURE);
    }
}

bool inside(Shape* s, Point* p) {
    switch (s->t) {
        case Circle:
            return pow(s->p.x - p->x, 2) + pow(s->p.y - p->y, 2)
                   <= pow(s->v.circle.r, 2);
        case Rectangle:
            return s->p.x <= p->x && p->x <= s->p.x + s->v.rectangle.w &&
                   s->p.y <= p->y && p->y <= s->p.y + s->v.rectangle.h;
        case Square:
            return s->p.x <= p->x && p->x <= s->p.x + s->v.square.l &&
                   s->p.y <= p->y && p->y <= s->p.y + s->v.square.l;
        default:
            fprintf(stderr, "unknown shape type: %d\n", s->t);
            exit(EXIT_FAILURE);
    }
}

void show_point(Point* p, FILE* stream) {
    if (!stream) stream = stdout;
    fprintf(stream, "{.x=%.2f, .y=%.2f}", p->x, p->y);
}

void show_shape(Shape* s, FILE* stream) {
    if (!stream) stream = stdout;
    switch (s->t) {
        case Circle:
            fprintf(stream, "{.t=Circle"
                            ", .p="); show_point(&s->p, stream);
            fprintf(stream, ", .v.circle={.r=%.2f}}", s->v.circle.r);
            break;
        case Rectangle:
            fprintf(stream, "{.t=Rectangle"
                            ", .p="); show_point(&s->p, stream);
            fprintf(stream, ", .v.rectangle={.w=%.2f, .h=%.2f}}",
                            s->v.rectangle.w, s->v.rectangle.h);
            break;
        case Square:
            fprintf(stream, "{.t=Square"
                            ", .p="); show_point(&s->p, stream);
            fprintf(stream, ", .v.square={.l=%.2f}}", s->v.square.l);
            break;
        default:
            fprintf(stderr, "unknown shape type: %d\n", s->t);
            exit(EXIT_FAILURE);
    }
}

bool test(void) {
    Shape shapes[3] = {
        (Shape){.t=Circle,    .p={.x= 0, .y= 0}, .v.circle={.r=3}},
        (Shape){.t=Rectangle, .p={.x=-1, .y= 2}, .v.rectangle={.w=2, .h=4}},
        (Shape){.t=Square,    .p={.x= 3, .y=-2}, .v.square={.l=3}},
    };
    Point points[5] = {
        (Point){.x=    0, .y=    0},
        (Point){.x=   10, .y=   10},
        (Point){.x=  1.5, .y=  1.5},
        (Point){.x=    0, .y=    4},
        (Point){.x=    3, .y=    0},
    };
    for(int i = 0; i < 3; ++i) {
        show_shape(&shapes[i], NULL);
        printf("\n\t -> area = %.2f", area(&shapes[i]));
        for(int j=0; j < 5; ++j) {
            printf("\n\t Point: "); show_point(&points[j], NULL);
            printf(inside(&shapes[i], &points[j]) ? " ✓" :  " ✗");
        }
        printf("\n");
    }

    return true;
}

void __attribute__((constructor)) init(void) {
}

int main(int argc, char* argv[]) {
    return test() ? EXIT_SUCCESS : EXIT_FAILURE;
}
