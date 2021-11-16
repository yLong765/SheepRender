//
// Created by yangyinlong01 on 2021/11/15.
//

#ifndef SHEEPRENDER_SR_VECTOR_H
#define SHEEPRENDER_SR_VECTOR_H

#include <string>
#include <cassert>
#include <cmath>

namespace SR {
    template<int n>
    struct vector {
        float data[n] = {0};

        vector() = default;

        float &operator[](const int i) {
            assert(i >= 0 && i < n);
            return data[i];
        }

        float operator[](const int i) const {
            assert(i >= 0 && i < n);
            return data[i];
        }
    };

    template<int n>
    vector<n> operator+(const vector<n> &lv, const vector<n> &rv) {
        vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = lv[i] + rv[i]; }
        return ret;
    }

    template<int n>
    vector<n> operator-(const vector<n> &lv, const vector<n> &rv) {
        vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = lv[i] - rv[i]; }
        return ret;
    }

    template<int n>
    float operator*(const vector<n> &lv, const vector<n> &rv) {
        float ret = 0;
        for (int i = 0; i < n; i++) { ret += lv[i] * rv[i]; }
        return ret;
    }

    template<int n>
    vector<n> operator*(const vector<n> &lv, const int &val) {
        vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = lv[i] * val; }
        return ret;
    }

    template<int n>
    vector<n> operator*(const int &num, const vector<n> &rv) {
        vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = num * rv[i]; }
        return ret;
    }

    template<int n>
    vector<n> operator/(const vector<n> &lv, const int &val) {
        vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = lv[i] / val; }
        return ret;
    }

    template<int n>
    std::ostream &operator<<(std::ostream &out, const vector<n> &v) {
        out << "(";
        for (int i = 0; i < n - 1; i++) { out << v[i] << ","; }
        out << v[n - 1] << ")";
        return out;
    }

    template<>
    struct vector<2> {
        float x, y;

        vector() = default;

        vector(float x, float y) : x(x), y(y) {}

        float &operator[](const int i) {
            assert(i >= 0 && i < 2);
            return i == 0 ? x : y;
        }

        float operator[](const int i) const {
            assert(i >= 0 && i < 2);
            return i == 0 ? x : y;
        }

        float sqr_magnitude() const { return (*this) * (*this); }

        float magnitude() const { return std::sqrt(sqr_magnitude()); }

        void normalize() { *this = (*this) / magnitude(); }

        static float dot(const vector &lv, const vector &rv) { return lv * rv; }
    };

    template<>
    struct vector<3> {
        float x, y, z;

        vector() = default;

        vector(float x, float y, float z) : x(x), y(y), z(z) {}

        float &operator[](const int i) {
            assert(i >= 0 && i < 3);
            return i == 0 ? x : i == 1 ? y : z;
        }

        float operator[](const int i) const {
            assert(i >= 0 && i < 3);
            return i == 0 ? x : i == 1 ? y : z;
        }

        float sqr_magnitude() const { return (*this) * (*this); }

        float magnitude() const { return std::sqrt(sqr_magnitude()); }

        void normalize() { *this = (*this) / magnitude(); }

        static float dot(const vector &lv, const vector &rv) { return lv * rv; }

        static vector cross(const vector &lv, const vector &rv) {
            vector ret{};
            ret.x = lv.y * rv.z + lv.z * rv.y;
            ret.y = lv.z * rv.x + lv.x * rv.z;
            ret.z = lv.x * rv.y + lv.y * rv.x;
            return ret;
        }
    };

    template<>
    struct vector<4> {
        float x, y, z, w;

        vector() = default;

        vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        float &operator[](const int i) {
            assert(i >= 0 && i < 4);
            return i == 0 ? x : i == 1 ? y : i == 2 ? z : w;
        }

        float operator[](const int i) const {
            assert(i >= 0 && i < 4);
            return i == 0 ? x : i == 1 ? y : i == 2 ? z : w;
        }

        float sqr_magnitude() const { return (*this) * (*this); }

        float magnitude() const { return std::sqrt(sqr_magnitude()); }

        void normalize() { *this = (*this) / magnitude(); }

        static float dot(const vector &lv, const vector &rv) { return lv * rv; }
    };

    typedef vector<2> vec2;
    typedef vector<3> vec3;
    typedef vector<4> vec4;
}

#endif //SHEEPRENDER_SR_VECTOR_H
