from os import pipe, fdopen, read, close
from cffi import FFI

with open('shape.h') as f:
    _cdef = f.read()

_ffi = FFI()
_ffi.cdef(_cdef)
_ffi.cdef('''
    FILE *fdopen(int fd, const char *mode);
    int fflush(FILE *stream);
''')
_so = _ffi.dlopen(None)
_so = _ffi.dlopen('./shape.so')


class Point:
    def __init__(self, x, y):
        self.shape = _ffi.new('Point*')
        self.shape.x = x
        self.shape.y = y

class Shape:
    def __init__(self, x, y):
        super().__init__()
        self.shape = _ffi.new('Shape*')
        self.shape.p.x = x
        self.shape.p.y = y

    @property
    def x(self):
        return self.shape.p.x

    @property
    def y(self):
        return self.shape.p.y

    @property
    def area(self):
        return _so.area(self.shape)

    def __contains__(self, other):
        return _so.inside(self.shape, other.point)

    def _show_shape(self):
        pipe_read, pipe_write = pipe()
        c_stream, py_stream = _so.fdopen(pipe_write, b'w'), fdopen(pipe_read)
        _so.show_shape(self.shape, c_stream); _so.fflush(c_stream)
        close(pipe_write)
        value = py_stream.read()
        close(pipe_read)
        return value

    def __repr__(self):
        return '<Shape {}>'.format(self._show_shape())

class Circle(Shape):
    def __init__(self, r, x, y):
        super().__init__(x, y)
        self.shape.t = _so.Circle
        self.shape.v.circle.r = r

    @property
    def r(self):
        return self.shape.v.circle.r

    def __repr__(self):
        return '<Circle {}>'.format(self._show_shape())

class Rectangle(Shape):
    def __init__(self, w, h, x, y):
        super().__init__(x, y)
        self.shape.t = _so.Rectangle
        self.shape.v.rectangle.w = w
        self.shape.v.rectangle.h = h

    @property
    def w(self):
        return self.shape.v.rectangle.w

    @property
    def h(self):
        return self.shape.v.rectangle.h

    def __repr__(self):
        return '<Rectangle {}>'.format(self._show_shape())

class Square(Shape):
    def __init__(self, l, x, y):
        super().__init__(x, y)
        self.shape.t = _so.Square
        self.shape.v.square.l = l

    @property
    def l(self):
        return self.shape.v.square.l

    def __repr__(self):
        return '<Square {}>'.format(self._show_shape())

class Point:
    def __init__(self, x, y):
        self.point = _ffi.new('Point*')
        self.point.x = x
        self.point.y = y

    def _show_point(self):
        pipe_read, pipe_write = pipe()
        c_stream, py_stream = _so.fdopen(pipe_write, b'w'), fdopen(pipe_read)
        _so.show_point(self.point, c_stream); _so.fflush(c_stream)
        close(pipe_write)
        value = py_stream.read()
        close(pipe_read)
        return value

    def __repr__(self):
        return '<Point {}>'.format(self._show_point())

if __name__ == '__main__':
    shapes = [
        Circle   (r=3,      x= 0, y= 0),
        Rectangle(w=2, h=4, x=-1, y= 2),
        Square   (l=3,      x= 3, y=-2),
    ]
    points = [
        Point(  0,   0),
        Point( 10,  10),
        Point(1.5, 1.5),
        Point(  0,   4),
        Point(  3,   0),
    ]

    for s in shapes:
        print(s)
        print('\t -> area:', s.area)
        for p in points:
            print('\t ', p, '✓' if p in s else '✘')
