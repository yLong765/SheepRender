//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_COLOR_H
#define SHEEPRENDER_SR_COLOR_H

namespace SR {
    typedef struct sr_color {
        float r, g, b, a;

        sr_color() = default;

        sr_color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {}

        float &operator[](const int i) {
            assert(i >= 0 && i < 4);
            return i == 0 ? r : i == 1 ? g : i == 2 ? b : a;
        }

        float operator[](const int i) const {
            assert(i >= 0 && i < 4);
            return i == 0 ? r : i == 1 ? g : i == 2 ? b : a;
        }
    } color;
}

#endif //SHEEPRENDER_SR_COLOR_H
