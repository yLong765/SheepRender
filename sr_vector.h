//
// Created by long on 2021/11/15.
//

#ifndef SHEEPRENDER_SR_VECTOR_H
#define SHEEPRENDER_SR_VECTOR_H

namespace SR {
    template<int n>
    struct sr_vector {
        float data[n] = {0};

        sr_vector() = default;

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
    sr_vector<n> operator+(const sr_vector<n> &lv, const sr_vector<n> &rv) {
        sr_vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = lv[i] + rv[i]; }
        return ret;
    }

    template<int n>
    sr_vector<n> operator-(const sr_vector<n> &lv, const sr_vector<n> &rv) {
        sr_vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = lv[i] - rv[i]; }
        return ret;
    }

    template<int n>
    float operator*(const sr_vector<n> &lv, const sr_vector<n> &rv) {
        float ret = 0;
        for (int i = 0; i < n; i++) { ret += lv[i] * rv[i]; }
        return ret;
    }

    template<int n>
    sr_vector<n> operator*(const sr_vector<n> &lv, const float &val) {
        sr_vector<n> ret{};
        for (int i = 0; i < n; i++) {
            ret[i] = lv[i] * val;
        }
        return ret;
    }

    template<int n>
    sr_vector<n> operator*(const float &num, const sr_vector<n> &rv) {
        sr_vector<n> ret;
        for (int i = 0; i < n; i++) { ret[i] = num * rv[i]; }
        return ret;
    }

    template<int n>
    sr_vector<n> operator/(const sr_vector<n> &lv, const float &val) {
        sr_vector<n> ret{};
        for (int i = 0; i < n; i++) { ret[i] = lv[i] / val; }
        return ret;
    }

    template<int n>
    std::ostream &operator<<(std::ostream &out, const sr_vector<n> &v) {
        out << "(";
        for (int i = 0; i < n - 1; i++) { out << v[i] << ","; }
        out << v[n - 1] << ")";
        return out;
    }

    template<>
    struct sr_vector<2> {
        float x, y;

        sr_vector() = default;

        sr_vector(float x, float y) : x(x), y(y) {}

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

        sr_vector normalize() { return *this = (*this) / magnitude(); }

        static float dot(const sr_vector &lv, const sr_vector &rv) { return lv * rv; }
    };

    template<>
    struct sr_vector<3> {
        float x, y, z;

        sr_vector() = default;

        sr_vector(float x, float y, float z) : x(x), y(y), z(z) {}

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

        sr_vector normalize() {
            float l = magnitude();
            if (l != 0.0f) {
                float inv = 1.0f / l;
                *this = (*this) * inv;
            }
            return *this;
        }

        static float dot(const sr_vector &lv, const sr_vector &rv) { return lv * rv; }

        static sr_vector cross(const sr_vector &lv, const sr_vector &rv) {
            sr_vector ret{};
            ret.x = lv.y * rv.z - lv.z * rv.y;
            ret.y = lv.z * rv.x - lv.x * rv.z;
            ret.z = lv.x * rv.y - lv.y * rv.x;
            return ret;
        }

        static sr_vector right() { return {1, 0, 0}; }

        static sr_vector left() { return {-1, 0, 0}; }

        static sr_vector up() { return {0, 1, 0}; }

        static sr_vector down() { return {0, -1, 0}; }

        static sr_vector forward() { return {0, 0, 1}; }

        static sr_vector back() { return {0, 0, -1}; }

        static sr_vector one() { return {1, 1, 1}; }

        static sr_vector zero() { return {0, 0, 0}; }
    };

    template<>
    struct sr_vector<4> {
        float x, y, z, w;

        sr_vector() = default;

        sr_vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        sr_vector<4>(sr_vector<3> v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

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

        sr_vector normalize() { return *this = (*this) / magnitude(); }

        static float dot(const sr_vector &lv, const sr_vector &rv) { return lv * rv; }
    };

    typedef sr_vector<2> vec2;
    typedef sr_vector<3> vec3;
    typedef sr_vector<4> vec4;
}

#endif //SHEEPRENDER_SR_VECTOR_H
