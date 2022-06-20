#ifndef MATRIX_H
#define MATRIX_H

#include "vec.h"

template<class T, unsigned int X, unsigned int Y>
class Matrix{
public:
    // Stored column major
    vec<T,Y> val[X];

    const T& at(int x, int y) const{
        return val[x][y];
    };
    T& at(int x, int y){
        return val[x][y];
    };
};

template<class T, unsigned int X, unsigned int Y>
vec<T,Y> operator*(const vec<T,Y> lhs, const Matrix<T,X,Y> rhs){
    vec<T,Y> result;
    for(int i=0; i<X; i++){
        vec<T,Y> temp = lhs.dot(rhs.val[i]);
        result = result + temp;
    }
    return result;
};

// Rotation matrix
class RotMatrix2d : public Matrix<double, 2, 2>{
private:
public:
    RotMatrix2d(){};
    RotMatrix2d(vec2d p);
    RotMatrix2d(double angle);

    void set_from_angle(double angle);
    void set_from_vec(vec2d);
};

#endif
