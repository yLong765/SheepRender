//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_COLOR_H
#define SHEEPRENDER_SR_COLOR_H

namespace SR {
    typedef struct sr_color {
        // color颜色（0~1）
        union {
            struct {
                float r, g, b, a;
            };
            sr_vector<4, float> c;
        };

        sr_color() = default;

        sr_color(float r, float g, float b, float a = 1.0f) {
            set(r, g, b, a);
        }

        float &operator[](const size_t i) {
            assert(i < 4);
            return c[i];
        }

        float operator[](const size_t i) const {
            assert(i < 4);
            return c[i];
        }

        // 转换到缓冲识别的颜色（0~255）
        // 每个占8位，一共32位。注意顺序是ARGB
        UINT get_pixel_color() const {
            sr_vector<4, UCHAR> oc{};
            for (size_t i = 0; i < 4; i++) {
                oc[i] = c[i] * 255.0f;
                oc[i] = clamp(oc[i], 0, 255);
            }
            return (oc.a << 24) | (oc.r << 16) | (oc.g << 8) | (oc.b);
        }

        // 设置颜色
        void set(float r, float g, float b, float a = 1) {
            this->r = r;
            this->b = b;
            this->g = g;
            this->a = a;
            normalized();
        }

        // 设置颜色
        void set(vec3f v, float a = 1) {
            set(v.r, v.g, v.b, a);
        }

        // 设置颜色
        void set(vec4f v) {
            set(v.r, v.g, v.b, v.a);
        }

        sr_color normalized() {
            for (int i = 0; i < 4; i++) {
                c[i] = clamp(c[i], 0.0f, 1.0f);
            }
            return *this;
        }
    } color;

    // lc *= x
    inline sr_color &operator*=(sr_color &lc, float &x) {
        lc.c *= x;
        lc.normalized();
        return lc;
    }

    // lc * x
    inline sr_color operator*(sr_color lc, float x) {
        sr_color ret;
        ret.c = lc.c * x;
        return ret.normalized();
    }

    // x * rc
    inline sr_color operator*(float x, sr_color rc) {
        sr_color ret;
        ret.c = x * rc.c;
        return ret.normalized();
    }
}

#endif //SHEEPRENDER_SR_COLOR_H
