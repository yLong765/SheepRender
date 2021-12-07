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
        float aspect;           // 宽高比
        sr_texture_2d *texture; // 颜色写入的贴图
        sr_camera *camera;      // 相对于哪个相机渲染
        float *z_buffer;        // 深度缓冲

        sr_base_render(sr_texture_2d *texture, sr_camera *camera, float *z_buffer) {
            this->texture = texture;
            this->width = texture->width;
            this->height = texture->height;
            this->aspect = math::aspect(width, height);
            this->camera = camera;
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

        // 绘制线
        void draw_line(int x1, int y1, int x2, int y2, color color) {
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

        // 绘制线
        void draw_line(vec2i v1, vec2i v2, color color) {
            draw_line(v1.x, v1.y, v2.x, v2.y, color);
        }

        // 绘制三角形线框
        void draw_triangle_wireframe(vec4f v1, vec4f v2, vec4f v3, color color) {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
            draw_line((int) v2.x, (int) v2.y, (int) v3.x, (int) v3.y, color);
            draw_line((int) v3.x, (int) v3.y, (int) v1.x, (int) v1.y, color);
        }

        // 归一化齐次坐标
        vec4f clip2ndc(vec4f clip_p) {
            // 1 / w
            float rhw = 1.0f / clip_p.w;
            // 归一化到[-1, 1] cvv
            return clip_p * rhw;
        }

        // 屏幕坐标
        vec2f ndc2screen(vec4f ndc_p) {
            return {(ndc_p.x + 1.0f) * width * 0.5f, (1.0f - ndc_p.y) * height * 0.5f};
        }

        // 整数屏幕坐标：加0.5的偏移取屏幕像素方格中心对齐
        vec2i screen_f2i(vec2f screen_pf) {
            return {(int) (screen_pf.x + 0.5f), (int) (screen_pf.y + 0.5f)};
        }

        vec2f clip2screen_f(vec4f clip_p) {
            return ndc2screen(clip2ndc(clip_p));
        }

        vec2i clip2screen_i(vec4f clip_p) {
            return screen_f2i(clip2screen_f(clip_p));
        }

        mat4x4f get_view_matrix() {
            return camera->get_look_at_matrix();
        }

        mat4x4f get_projection_matrix() {
            return camera->get_perspective_matrix(aspect);
        }
    } base_render;
}

#endif //SHEEPRENDER_SR_BASE_RENDER_H
