//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_RENDER_H
#define SHEEPRENDER_SR_RENDER_H

namespace SR {
    enum RENDER_MESH_TYPE {
        BARYCENTRIC_COORDINATE = 0,
        EDGE_EQUATION = 1,
        EDGE_WALKING = 2,
        // 深度渲染
        DEPTH_RENDER = 10,
    };

    enum RENDER_WIREFRAME_TYPE {
        AXIS_WIREFRAME = 0,
    };

    // 渲染类
    typedef struct sr_render : public sr_singleton<sr_render> {
    protected:
        int width;              // 宽
        int height;             // 高
        sr_texture_2d *texture; // 颜色写入的贴图
        sr_camera *camera;      // 相对于哪个相机渲染
        sr_light *light;        // 相对于哪个灯光渲染
        float *z_buffer;        // 深度缓冲

    public:
        sr_render() = default;

        sr_render(sr_texture_2d *texture) {
            this->width = texture->width;
            this->height = texture->height;
            this->texture = texture;
            this->z_buffer = (float *) malloc(sizeof(float) * width * height);
        }

        sr_render(sr_texture_2d *texture, sr_camera *camera, sr_light *light) {
            this->width = texture->width;
            this->height = texture->height;
            this->texture = texture;
            this->camera = camera;
            this->light = light;
            this->z_buffer = (float *) malloc(sizeof(float) * width * height);
        }

        // 设置贴图
        void set_texture(sr_texture_2d *texture) {
            this->texture = texture;
            this->width = texture->width;
            this->height = texture->height;
            this->z_buffer = (float *) malloc(sizeof(float) * width * height);
        }

        // 设置相机
        void set_camera(sr_camera *camera) {
            this->camera = camera;
        }

        // 设置灯光
        void set_light(sr_light *light) {
            this->light = light;
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
        void draw_line(vec4f v1, vec4f v2, color color) {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
        }

        // 绘制三角形线框
        void draw_triangle_wireframe(vec4f v1, vec4f v2, vec4f v3, color color) {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
            draw_line((int) v2.x, (int) v2.y, (int) v3.x, (int) v3.y, color);
            draw_line((int) v3.x, (int) v3.y, (int) v1.x, (int) v1.y, color);
        }

        // 绘制物体的本地坐标轴
//        void draw_axis(sr_object obj) {
//            mat4x4f model = obj.transform.get_world_matrix();
//            mat4x4f view = camera->get_look_at_matrix();
//            mat4x4f projection = camera->get_perspective_matrix(math::aspect(width, height));
//            // z forward
//            vec4f mpf = obj.transform.position.xyz1();
//            vec4f fp = vec4f(0, 0, 3, 1) * model + mpf;
//            vec4f cpf = mpf * view * projection;
//            fp = fp * view * projection;
//            vec4f ndc = clip2ndc(cpf);
//            vec2f spf = ndc2screen(ndc);
//            fp = camera->homogenize(fp, width, height);
//            draw_line(p, fp, color(0, 0, 1));
//            // y up
//            p = obj.transform.position.xyz1();
//            fp = vec4f(0, 3, 0, 1) * model + p;
//            p = p * view * projection;
//            fp = fp * view * projection;
//            p = camera->homogenize(p, width, height);
//            fp = camera->homogenize(fp, width, height);
//            draw_line(p, fp, color(0, 1, 0));
//            // x right
//            p = obj.transform.position.xyz1();
//            fp = vec4f(3, 0, 0, 1) * model + p;
//            p = p * view * projection;
//            fp = fp * view * projection;
//            p = camera->homogenize(p, width, height);
//            fp = camera->homogenize(fp, width, height);
//            draw_line(p, fp, color(1, 0, 0));
//        }

        void draw_mesh(RENDER_MESH_TYPE type = BARYCENTRIC_COORDINATE) {
            sr_mesh_render *render = nullptr;
            switch (type) {
                case BARYCENTRIC_COORDINATE:
                    render = new sr_barycentric_render(texture, camera, light, z_buffer);
                    break;
                case EDGE_EQUATION:
                    break;
                case EDGE_WALKING:
                    break;
                case DEPTH_RENDER:
                    render = new sr_depth_render(texture, camera, light, z_buffer);
                    break;
            }
            if (render != nullptr) {
                render->draw();
            }
        }

        void draw_wireframe(RENDER_WIREFRAME_TYPE type) {

        }
    } render;
}

#endif //SHEEPRENDER_SR_RENDER_H