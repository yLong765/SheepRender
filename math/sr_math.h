//
// Created by long on 2021/11/20.
//

#ifndef SHEEPRENDER_SR_MATH_H
#define SHEEPRENDER_SR_MATH_H

namespace SR {
    typedef struct sr_math {
        // 利用重心坐标判断点是否在三角形内部
        static vec3f barycentric(const vec2i A, const vec2i B, const vec2i C, const vec2i P) {
            vec3f x(B.x - A.x, C.x - A.x, A.x - P.x);
            vec3f y(B.y - A.y, C.y - A.y, A.y - P.y);

            // u 向量和 x y 向量的点积为 0，所以 x y 向量叉乘可以得到 u 向量
            vec3f u = vec_cross(x, y);

            // 所以 u.z 绝对值小于 1 意味着三角形退化了，直接舍弃
            if (std::abs(u.z) < 1) {
                return {-1.0f, 1.0f, 1.0f};
            }
            return {1.0f - (u.x + u.y) / u.z, u.x / u.z, u.y / u.z};
        }

        // 利用重心坐标判断点是否在三角形内部
        static vec3f barycentric(const vec2i *Ps, const vec2i P) {
            return barycentric(Ps[0], Ps[1], Ps[2], P);
        }

        // PI数据
        static float pi;

        // 弧度转角度
        static float deg2rad(float deg) {
            return (pi / 180) * deg;
        }

        // x控制在min和max之间
        static float clamp(float x, float min, float max) {
            return (x < min) ? min : ((x > max) ? max : x);
        }

        // x控制在min和max之间
        static int clamp(int x, int min, int max) {
            return (x < min) ? min : ((x > max) ? max : x);
        }

        // 屏幕宽高比
        static float aspect(int w, int h) {
            return (float) w / (float) h;
        }
    } math;

    float sr_math::pi = 3.1415926f;
}

#endif //SHEEPRENDER_SR_MATH_H
