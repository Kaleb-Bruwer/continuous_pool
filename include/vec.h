#ifndef VEC2_H
#define VEC2_H

#include <cmath>

template <class T, unsigned int dims>
class vec{
public:
    T val[dims];

    // an int vec would also return double for magnitude
    double magnitude() const{
        double total = 0;
        for(int i=0; i<dims; i++){
            total += val[i] * val[i];
        }
        return sqrt(total);
    }

    bool is_zero() const{
        for(int i=0; i<dims; i++){
            if(val[i] != 0)
                return false;
        }
        return true;
    };

    const T& operator[](const int index) const
        {return val[index];};
    T& operator[](const int index)
        {return val[index];};
};

template <class T, unsigned int dims>
vec<T, dims> operator+(const vec<T,dims> lhs, const vec<T,dims> rhs){
    vec<T, dims> result;
    for(int i=0; i<dims; i++){
        result.val[i] = lhs.val[i] + rhs.val[i];
    }

    return result;
};

template <class T, unsigned int dims>
vec<T, dims> operator*(const vec<T,dims> lhs, double rhs){
    vec<T, dims> result;
    for(int i=0; i<dims; i++){
        result.val[i] = lhs.val[i] * rhs;
    }

    return result;
};

template <class T, unsigned int dims>
vec<T, dims> operator-(const vec<T,dims> lhs, const vec<T,dims> rhs){
    vec<T, dims> result;
    for(int i=0; i<dims; i++){
        result.val[i] = lhs.val[i] - rhs.val[i];
    }

    return result;
}

typedef vec<double, 2> vec2d;

#endif
