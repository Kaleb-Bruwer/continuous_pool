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
vec<T,X> operator*(const vec<T,Y> lhs, const Matrix<T,X,Y> rhs){
    vec<T,X> result;

    for(int i=0; i<X; i++){
        T temp = 0;

        for(int j=0; j<Y; j++)
            temp += lhs[j] * rhs.at(i,j);
        result[i] = temp;
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
