//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_TEXTURE_2D_H
#define SHEEPRENDER_SR_TEXTURE_2D_H

namespace SR {
    typedef struct sr_texture_2d {
        int width, height;
        std::vector<sr_color> colors;

        sr_texture_2d(int width, int height) : width(width), height(height) {
            colors = std::vector<color>(width * height, color(0, 0, 0, 1));
        }

        sr_color &operator[](const int i) {
            assert(i >= 0 && i < colors.size());
            return colors[i];
        }

        sr_color operator[](const int i) const {
            assert(i >= 0 && i < colors.size());
            return colors[i];
        }
    } texture_2d;
}

#endif //SHEEPRENDER_SR_TEXTURE_2D_H
