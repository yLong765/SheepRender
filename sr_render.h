//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_RENDER_H
#define SHEEPRENDER_SR_RENDER_H

namespace SR {
    typedef struct sr_render {
        int width;
        int height;
        sr_texture_2d *texture;
        float *z_buffer;

        sr_render(sr_texture_2d *texture) {
            this->width = texture->width;
            this->height = texture->height;
            this->texture = texture;
            this->z_buffer = (float *) malloc(sizeof(float) * width * height);
        }

        void set_color(int index, color color) const {
            (*texture)[index] = color;
        }

        void clear_color(color color) const {
            int pixel_count = width * height;
            for (int i = 0; i < pixel_count; i++) {
                set_color(i, color);
            }
        }

        void draw_pixel(int x, int y, color color) const {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                set_color(y * width + x, color);
            }
        }

        void draw_line(int x1, int y1, int x2, int y2, color color) const {
            int x, y, rem = 0;
            if (x1 == x2 && y1 == y2) {
                draw_pixel(x1, y1, color);
            } else if (x1 == x2) {
                int inc = (y1 <= y2) ? 1 : -1;
                for (y = y1; y != y2; y += inc) draw_pixel(x1, y, color);
                draw_pixel(x2, y2, color);
            } else if (y1 == y2) {
                int inc = (x1 <= x2) ? 1 : -1;
                for (x = x1; x != x2; x += inc) draw_pixel(x, y1, color);
                draw_pixel(x2, y2, color);
            } else {
                int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
                int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
                if (dx >= dy) {
                    if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
                    for (x = x1, y = y1; x <= x2; x++) {
                        draw_pixel(x, y, color);
                        rem += dy;
                        if (rem >= dx) {
                            rem -= dx;
                            y += (y2 >= y1) ? 1 : -1;
                            draw_pixel(x, y, color);
                        }
                    }
                    draw_pixel(x2, y2, color);
                } else {
                    if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
                    for (x = x1, y = y1; y <= y2; y++) {
                        draw_pixel(x, y, color);
                        rem += dx;
                        if (rem >= dy) {
                            rem -= dy;
                            x += (x2 >= x1) ? 1 : -1;
                            draw_pixel(x, y, color);
                        }
                    }
                    draw_pixel(x2, y2, color);
                }
            }
        }

        void draw_triangle_wireframe(vec4 v1, vec4 v2, vec4 v3, color color) const {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
            draw_line((int) v2.x, (int) v2.y, (int) v3.x, (int) v3.y, color);
            draw_line((int) v3.x, (int) v3.y, (int) v1.x, (int) v1.y, color);
        }
    } render;
}

#endif //SHEEPRENDER_SR_RENDER_H
