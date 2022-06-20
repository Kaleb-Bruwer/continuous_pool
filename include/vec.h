#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <cstring>

template <class T, unsigned int dims>
class vec{
public:
    T val[dims];

    // subscript operator, both versions are needed so it can be used within const funcs
    const T& operator[](const int index) const
        {return val[index];};
    T& operator[](const int index)
        {return val[index];};

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

    vec<T, dims> dot(const vec<T,dims> rhs) const{
        vec<T, dims> result;
        for(int i=0; i<dims; i++){
            result[i] = val[i] * rhs[i];
        }
        return result;
    }

    void operator+=(const vec<T,dims> rhs){
        for(int i=0; i<dims; i++){
            val[i] += rhs[i];
        }
    }
    void operator-=(const vec<T,dims> rhs){
        for(int i=0; i<dims; i++){
            val[i] -= rhs[i];
        }
    }

    vec<T,dims>& operator=(const vec<T,dims>& rhs){
        if(this == &rhs)
            return *this;

        memcpy(&val, &rhs.val, sizeof(val));
        return *this;
    }

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
vec<T, dims> operator*(const vec<T,dims> lhs, const double rhs){
    vec<T, dims> result;
    for(int i=0; i<dims; i++){
        result.val[i] = lhs.val[i] * rhs;
    }

    return result;
};

template <class T, unsigned int dims>
vec<T, dims> operator*(const double lhs, const vec<T,dims> rhs){
    return rhs * lhs;
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
typedef vec<int, 2> vec2i;

#endif
