//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_COLOR_H
#define SHEEPRENDER_SR_COLOR_H

namespace SR {
    typedef struct sr_color {
        sr_vector<4, float> c{};

        sr_color() = default;

        sr_color(float r, float g, float b, float a = 1.0f) {
            c.r = r;
            c.g = g;
            c.b = b;
            c.a = a;
        }

        float &operator[](const size_t i) {
            assert(i < 4);
            return c[i];
        }

        float operator[](const size_t i) const {
            assert(i < 4);
            return c[i];
        }

        UINT get_pixel_color() const {
            sr_vector<4, UCHAR> oc{};
            for (size_t i = 0; i < 4; i++) {
                oc[i] = c[i] * 255.0f;
                oc[i] = clamp(oc[i], 0, 255);
            }
            return (oc.a << 24) | (oc.r << 16) | (oc.g << 8) | (oc.b);
        }

        void set(float r, float g, float b, float a = 1) {
            c.r = r;
            c.b = b;
            c.g = g;
            c.a = a;
        }
    } color;
}

#endif //SHEEPRENDER_SR_COLOR_H
