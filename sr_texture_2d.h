//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_TEXTURE_2D_H
#define SHEEPRENDER_SR_TEXTURE_2D_H

namespace SR {
    // 贴图类型
    enum TEXTURE_2D_TYPE {
        TEX_RGB,
        TEX_RGBA,
    };

    // 贴图类
    typedef struct sr_texture_2d {
        int width;  // 宽
        int height; // 高
        TEXTURE_2D_TYPE type;
        std::vector<sr_color> colors;   // 颜色

        sr_texture_2d(int width, int height, TEXTURE_2D_TYPE type = TEX_RGB) : width(width), height(height) {
            colors = std::vector<sr_color>(width * height, sr_color(0, 0, 0, 1));
            this->type = type;
        }

        sr_color &operator[](const int i) {
            assert(i >= 0 && i < colors.size());
            return colors[i];
        }

        sr_color operator[](const int i) const {
            assert(i >= 0 && i < colors.size());
            return colors[i];
        }

        UINT get_pixel(int i) {
            return colors[i].get_pixel_color();
        }

        UINT get_pixel(int x, int y) {
            return colors[x + y * width].get_pixel_color();
        }

        // 设置位置的颜色（一维）
        void set(int i, sr_color color) {
            colors[i] = color;
            if (type == TEX_RGB) {
                colors[i].a = 1;
            }
        }

        // 设置位置颜色（二维）
        void set(int x, int y, sr_color color) {
            set(x + y * width, color);
        }
    } texture_2d;
}

#endif //SHEEPRENDER_SR_TEXTURE_2D_H
