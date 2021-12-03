//
// Created by long on 2021/12/3.
//

#ifndef SHEEPRENDER_SR_BASE_RENDER_H
#define SHEEPRENDER_SR_BASE_RENDER_H

namespace SR {
    typedef struct sr_base_render {
    protected:
        int width;              // 宽
        int height;             // 高
        sr_texture_2d *texture; // 颜色写入的贴图
        float *z_buffer;        // 深度缓冲

        sr_base_render(sr_texture_2d *texture, float *z_buffer) {
            this->texture = texture;
            this->width = texture->width;
            this->height = texture->height;
            this->z_buffer = z_buffer;
        }

        // 清空颜色
        void clear_color(color color) const {
            int pixel_count = width * height;
            for (int i = 0; i < pixel_count; i++) {
                texture->set(i, color);
            }
        }

        // 清空深度缓冲
        void clear_z_buffer() {
            int pixel_count = width * height;
            for (int i = 0; i < pixel_count; i++) {
                z_buffer[i] = FLT_MAX;
            }
        }

        // 绘制像素点
        void draw_pixel(int x, int y, color color) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                texture->set(x, y, color);
            }
        }
    } base_render;
}

#endif //SHEEPRENDER_SR_BASE_RENDER_H
