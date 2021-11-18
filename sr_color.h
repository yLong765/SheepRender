//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_COLOR_H
#define SHEEPRENDER_SR_COLOR_H

namespace SR {
    typedef struct sr_color {
        float r, g, b, a;

        sr_color() = default;

        sr_color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

        float &operator[](const int i) {
            assert(i >= 0 && i < 4);
            return i == 0 ? r : i == 1 ? g : i == 2 ? b : a;
        }

        float operator[](const int i) const {
            assert(i >= 0 && i < 4);
            return i == 0 ? r : i == 1 ? g : i == 2 ? b : a;
        }

        UINT get_pixel_color() const {
            UCHAR R = (UCHAR) (r * 255.0f);
            UCHAR G = (UCHAR) (g * 255.0f);
            UCHAR B = (UCHAR) (b * 255.0f);
            UCHAR A = (UCHAR) (a * 255.0f);
            R = clamp(R, 0, 255);
            G = clamp(G, 0, 255);
            B = clamp(B, 0, 255);
            A = clamp(A, 0, 255);
            return (A << 24) | (R << 16) | (G << 8) | (B);
        }
    } color;
}

#endif //SHEEPRENDER_SR_COLOR_H
