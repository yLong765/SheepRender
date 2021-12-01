//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_TEXTURE_2D_H
#define SHEEPRENDER_SR_TEXTURE_2D_H

namespace SR {
    // 贴图类
    typedef struct sr_texture_2d {
        int width;  // 宽
        int height; // 高
        std::vector<sr_color> colors;   // 颜色

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

        // 设置位置的颜色（一维）
        void set(int i, sr_color color) {
            colors[i] = color;
        }

        // 设置位置颜色（二维）
        void set(int x, int y, sr_color color) {
            colors[x + y * width] = color;
        }
    } texture_2d;
}

#endif //SHEEPRENDER_SR_TEXTURE_2D_H
