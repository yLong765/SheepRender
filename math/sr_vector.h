//
// Created by long on 2021/11/15.
//

#ifndef SHEEPRENDER_SR_VECTOR_H
#define SHEEPRENDER_SR_VECTOR_H

namespace SR {
    template<size_t N, typename T>
    struct sr_vector {
        T data[N];

        inline sr_vector() = default;

        inline sr_vector(const sr_vector<N, T> &v) { for (size_t i = 0; i < N; ++i) data[i] = v[i]; }

        inline float &operator[](const size_t i) {
            assert(i < N);
            return data[i];
        }

        inline float const &operator[](const size_t i) const {
            assert(i < N);
            return data[i];
        }
    };

    template<typename T>
    struct sr_vector<1, T> {
        union {
            T x;
            T u;
            T data[1];
        };

        inline sr_vector() = default;

        inline sr_vector(T x) : x(x) {}

        template<size_t N>
        inline sr_vector(const sr_vector<N, T> &v) {
            assert(N > 0);
            x = v.x;
        }

        inline T &operator[](const size_t i) {
            assert(i < 1);
            return data[i];
        }

        inline const T &operator[](const size_t i) const {
            assert(i < 1);
            return data[i];
        }

        inline sr_vector<2, T> x1() const { return {x, 1}; }

        inline sr_vector<3, T> x11() const { return {x, 1, 1}; }

        inline sr_vector<4, T> x111() const { return {x, 1, 1, 1}; }
    };

    template<typename T>
    struct sr_vector<2, T> {
        union {
            struct {
                T x, y;
            };
            struct {
                T u, v;
            };
            T data[2];
        };

        inline sr_vector() = default;

        inline sr_vector(T x, T y) : x(x), y(y) {}

        template<size_t N>
        inline sr_vector(const sr_vector<N, T> &v) {
            assert(N > 1);
            x = v.x;
            y = v.y;
        }

        inline T &operator[](const size_t i) {
            assert(i < 2);
            return data[i];
        }

        inline const T &operator[](const size_t i) const {
            assert(i < 2);
            return data[i];
        }

        inline sr_vector<2, T> xy() const { return {x, y}; }

        inline sr_vector<3, T> xy1() const { return {x, y, 1}; }

        inline sr_vector<4, T> xy11() const { return {x, y, 1, 1}; }
    };

    template<typename T>
    struct sr_vector<3, T> {
        union {
            struct {
                T x, y, z;
            };
            struct {
                T r, g, b;
            };
            T data[3];
        };

        inline sr_vector() = default;

        inline sr_vector(T x, T y, T z) : x(x), y(y), z(z) {}

        template<size_t N>
        inline sr_vector(const sr_vector<N, T> &v) {
            assert(N > 2);
            x = v.x;
            y = v.y;
            z = v.z;
        }

        inline T &operator[](const size_t i) {
            assert(i < 3);
            return data[i];
        }

        inline const T &operator[](const size_t i) const {
            assert(i < 3);
            return data[i];
        }

        inline sr_vector<2, T> xy() const { return {x, y}; }

        inline sr_vector<3, T> xyz() const { return {x, y, z}; }

        inline sr_vector<4, T> xyz1() const { return {x, y, z, 1}; }

        static sr_vector<3, T> right() { return {1, 0, 0}; }

        static sr_vector<3, T> left() { return {-1, 0, 0}; }

        static sr_vector<3, T> up() { return {0, 1, 0}; }

        static sr_vector<3, T> down() { return {0, -1, 0}; }

        static sr_vector<3, T> forward() { return {0, 0, 1}; }

        static sr_vector<3, T> back() { return {0, 0, -1}; }

        static sr_vector<3, T> one() { return {1, 1, 1}; }

        static sr_vector<3, T> zero() { return {0, 0, 0}; }
    };

    template<typename T>
    struct sr_vector<4, T> {
        union {
            struct {
                T x, y, z, w;
            };
            struct {
                T r, g, b, a;
            };
            T data[4];
        };

        inline sr_vector() = default;

        inline sr_vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        template<size_t N>
        inline sr_vector(const sr_vector<N, T> &v) {
            assert(N > 3);
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }

        inline T &operator[](const size_t i) {
            assert(i < 4);
            return data[i];
        }

        inline const T &operator[](const size_t i) const {
            assert(i < 4);
            return data[i];
        }

        inline sr_vector<2, T> xy() const { return {x, y}; }

        inline sr_vector<3, T> xyz() const { return {x, y, z}; }

        inline sr_vector<4, T> xyzw() const { return {x, y, z, w}; }
    };

    //// 重写向量运算符 ////

    // -v
    template<size_t N, typename T>
    inline sr_vector<N, T> operator-(const sr_vector<N, T> &v) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = -v[i];
        return ret;
    }

    // lv + rv
    template<size_t N, typename T>
    inline sr_vector<N, T> operator+(const sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = lv[i] + rv[i];
        return ret;
    }

    // lv - rv
    template<size_t N, typename T>
    inline sr_vector<N, T> operator-(const sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = lv[i] - rv[i];
        return ret;
    }

    // lv * rv，各个元素分别相乘
    template<size_t N, typename T>
    inline sr_vector<N, T> operator*(const sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = lv[i] * rv[i];
        return ret;
    }

    // lv / rv，各个元素分别相除
    template<size_t N, typename T>
    inline sr_vector<N, T> operator/(const sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = lv[i] / rv[i];
        return ret;
    }

    // x * rv
    template<size_t N, typename T>
    inline sr_vector<N, T> operator*(const float &x, const sr_vector<N, T> &rv) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = x * rv[i];
        return ret;
    }

    // lv * x
    template<size_t N, typename T>
    inline sr_vector<N, T> operator*(const sr_vector<N, T> &lv, const float &x) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = lv[i] * x;
        return ret;
    }

    // lv / x
    template<size_t N, typename T>
    inline sr_vector<N, T> operator/(const sr_vector<N, T> &lv, const float &x) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = lv[i] / x;
        return ret;
    }

    // x / rv
    template<size_t N, typename T>
    inline sr_vector<N, T> operator/(const float &x, const sr_vector<N, T> &rv) {
        sr_vector<N, T> ret;
        for (size_t i = 0; i < N; i++) ret[i] = x / rv[i];
        return ret;
    }

    // lv += rv
    template<size_t N, typename T>
    inline sr_vector<N, T> &operator+=(sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        for (size_t i = 0; i < N; i++) lv[i] += rv[i];
        return lv;
    }

    // lv -= rv
    template<size_t N, typename T>
    inline sr_vector<N, T> &operator-=(sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        for (size_t i = 0; i < N; i++) lv[i] -= rv[i];
        return lv;
    }

    // lv *= x
    template<size_t N, typename T>
    inline sr_vector<N, T> &operator*=(sr_vector<N, T> &lv, const T &x) {
        for (size_t i = 0; i < N; i++) lv[i] *= x;
        return lv;
    }

    // lv *= rv
    template<size_t N, typename T>
    inline sr_vector<N, T> &operator*=(sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        for (size_t i = 0; i < N; i++) lv[i] *= rv[i];
        return lv;
    }

    // lv /= rv
    template<size_t N, typename T>
    inline sr_vector<N, T> &operator/=(sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        for (size_t i = 0; i < N; i++) lv[i] /= rv[i];
        return lv;
    }

    // lv *= x
    template<size_t N, typename T>
    inline sr_vector<N, T> &operator-=(sr_vector<N, T> &lv, const T &x) {
        for (size_t i = 0; i < N; i++) lv[i] *= x;
        return lv;
    }

    // lv /= x
    template<size_t N, typename T>
    inline sr_vector<N, T> &operator/=(sr_vector<N, T> &lv, const T &x) {
        for (size_t i = 0; i < N; i++) lv[i] /= x;
        return lv;
    }

    // cout <<
    template<size_t N, typename T>
    std::ostream &operator<<(std::ostream &out, const sr_vector<N, T> &v) {
        out << "(";
        for (size_t i = 0; i < N - 1; i++) out << v[i] << ",";
        out << v[N - 1] << ")";
        return out;
    }

    //// 向量函数 ////

    // 点乘
    template<size_t N, typename T>
    inline T vec_dot(const sr_vector<N, T> &lv, const sr_vector<N, T> &rv) {
        T ret = 0.0f;
        for (size_t i = 0; i < N; i++) ret += lv[i] * rv[i];
        return ret;
    }

    // 二维叉乘
    template<typename T>
    inline T vec_cross(const sr_vector<2, T> &lv, const sr_vector<2, T> &rv) {
        return lv.x * rv.y - lv.y * rv.x;
    }

    // 三维叉乘
    template<typename T>
    inline sr_vector<3, T> vec_cross(const sr_vector<3, T> &lv, const sr_vector<3, T> &rv) {
        return {lv.y * rv.z - lv.z * rv.y, lv.z * rv.x - lv.x * rv.z, lv.x * rv.y - lv.y * rv.x};
    }

    // 四维叉乘
    template<typename T>
    inline sr_vector<4, T> vec_cross(const sr_vector<4, T> &lv, const sr_vector<4, T> &rv) {
        return {lv.y * rv.z - lv.z * rv.y, lv.z * rv.x - lv.x * rv.z, lv.x * rv.y - lv.y * rv.x, lv.w};
    }

    // 向量距离平方
    template<size_t N, typename T>
    inline T vec_sqr_magnitude(const sr_vector<N, T> v) {
        return vec_dot(v, v);
    }

    // 向量长度
    template<size_t N, typename T>
    inline T vec_magnitude(const sr_vector<N, T> v) {
        return std::sqrt(vec_sqr_magnitude(v));
    }

    // 单位化向量
    template<size_t N, typename T>
    inline sr_vector<N, T> vec_normalize(const sr_vector<N, T> v) {
        return v / vec_magnitude(v);
    }

    // 反射向量
    template<size_t N, typename T>
    inline sr_vector<N, T> vec_reflect(const sr_vector<N, T> l, const sr_vector<N, T> n) {
        return vec_normalize(l - 2 * n * vec_dot(l, n));
    }

    typedef sr_vector<1, float> vec1f;
    typedef sr_vector<2, float> vec2f;
    typedef sr_vector<3, float> vec3f;
    typedef sr_vector<4, float> vec4f;
    typedef sr_vector<2, int> vec2i;
    typedef sr_vector<3, int> vec3i;
}

#endif //SHEEPRENDER_SR_VECTOR_H
