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
        sr_camera *camera;

        sr_render(sr_texture_2d *texture, sr_camera *camera) {
            this->width = texture->width;
            this->height = texture->height;
            this->texture = texture;
            this->z_buffer = (float *) malloc(sizeof(float) * width * height);
            this->camera = camera;
        }

        void clear_color(color color) const {
            int pixel_count = width * height;
            for (int i = 0; i < pixel_count; i++) {
                texture->set(i, color);
            }
        }

        void clear_z_buffer() {
            int pixel_count = width * height;
            for (int i = 0; i < pixel_count; i++) {
                z_buffer[i] = FLT_MAX;
            }
        }

        void draw_pixel(int x, int y, color color) const {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                texture->set(x, y, color);
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

        void draw_line(vec4f v1, vec4f v2, color color) const {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
        }

        void draw_triangle_wireframe(vec4f v1, vec4f v2, vec4f v3, color color) const {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
            draw_line((int) v2.x, (int) v2.y, (int) v3.x, (int) v3.y, color);
            draw_line((int) v3.x, (int) v3.y, (int) v1.x, (int) v1.y, color);
        }

        void draw_wireframe(sr_object obj, sr_color color) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix();
            mat4x4f mvp = model * view * projection;
            for (int i = 0; i < obj.mesh.triangles.size(); i += 3) {
                vec4f screen_point[3];
                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh.triangles[i + j];
                    screen_point[j] = camera->homogenize(obj.mesh.vertices[id].xyz1() * mvp);
                }
                draw_triangle_wireframe(screen_point[0], screen_point[1], screen_point[2], color);
            }
        }

        void draw_normal(sr_object obj, sr_color color) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix();
            mat4x4f mvp = model * view * projection;
            for (int i = 0; i < obj.mesh.triangles.size(); i += 3) {
                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh.triangles[i + j];
                    vec4f screen_point = camera->homogenize(obj.mesh.vertices[id].xyz1() * mvp);
                    vec4f screen_normal = camera->homogenize(
                            (obj.mesh.vertices[id] + obj.mesh.normals[id]).xyz1() * mvp);
                    draw_line(screen_point, screen_normal, color);
                }
            }
        }

        void draw_js_normal(sr_object obj, sr_color color) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix();
            mat4x4f mvp = model * view * projection;
            for (int i = 0; i < obj.mesh.triangles.size(); i += 3) {
                vec4f mpf[3];
                vec4f cpf[3];
                vec4f spf[3];
                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh.triangles[i + j];
                    mpf[j] = obj.mesh.vertices[id].xyz1();
                    cpf[j] = mpf[j] * mvp;
                    spf[j] = camera->homogenize(cpf[j]);
                }
                vec4f v01 = mpf[1] - mpf[0];
                vec4f v02 = mpf[2] - mpf[0];
                vec4f normal = vec_normalize(vec_cross(v01, v02));
                vec4f npf[3] = {mpf[0] + normal, mpf[1] + normal, mpf[2] + normal};
                for (int j = 0; j < 3; j++) {
                    npf[j] = npf[j] * mvp;
                    npf[j] = camera->homogenize(npf[j]);
                    draw_line(spf[j], npf[j], color);
                }
            }
        }

        void draw_axis(sr_object obj) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix();
            // z forward
            vec4f p = obj.transform.position.xyz1();
            vec4f fp = vec4f(0, 0, 1, 1) * model + p;
            p = p * view * projection;
            fp = fp * view * projection;
            p = camera->homogenize(p);
            fp = camera->homogenize(fp);
            draw_line(p, fp, color(0, 0, 1));
            // y up
            p = obj.transform.position.xyz1();
            fp = vec4f(0, 1, 0, 1) * model + p;
            p = p * view * projection;
            fp = fp * view * projection;
            p = camera->homogenize(p);
            fp = camera->homogenize(fp);
            draw_line(p, fp, color(0, 1, 0));
            // x right
            p = obj.transform.position.xyz1();
            fp = vec4f(1, 0, 0, 1) * model + p;
            p = p * view * projection;
            fp = fp * view * projection;
            p = camera->homogenize(p);
            fp = camera->homogenize(fp);
            draw_line(p, fp, color(1, 0, 0));
        }

        void draw(sr_object obj) const {
            shader *shader = obj.mesh.shader;
            shader->mat_model = obj.transform.get_world_matrix();
            shader->mat_view = camera->get_look_at_matrix();
            shader->mat_proj = camera->get_perspective_matrix();
            shader->mat_mvp = shader->mat_model * shader->mat_view * shader->mat_proj;
            for (int i = 0; i < obj.mesh.triangles.size(); i += 3) {
                vec4f cpf[3];   // 齐次空间坐标
                vec2f spf[3];   // 屏幕坐标
                vec2i spi[3];   // 整数屏幕坐标

                vec2i box_min(width - 1, height - 1);
                vec2i box_max(0, 0);

                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh.triangles[i + j];
                    cpf[j] = obj.mesh.shader->vert({obj.mesh.vertices[id], obj.mesh.normals[id]});

                    // 1 / w
                    float rhw = 1.0f / cpf[j].w;
                    // 归一化到[-1, 1] cvv
                    cpf[j] *= rhw;

                    // 屏幕坐标
                    spf[j].x = (cpf[j].x + 1.0f) * width * 0.5f;
                    spf[j].y = (1.0f - cpf[j].y) * height * 0.5f;

                    // 整数屏幕坐标：加0.5的偏移取屏幕像素方格中心对齐
                    spi[j].x = (int) (spf[j].x + 0.5f);
                    spi[j].y = (int) (spf[j].y + 0.5f);

                    // 三角形外接矩形
                    box_min.x = std::max(0, std::min(box_min.x, spi[j].x));
                    box_min.y = std::max(0, std::min(box_min.y, spi[j].y));
                    box_max.x = std::min(width - 1, std::max(box_max.x, spi[j].x));
                    box_max.y = std::min(height - 1, std::max(box_max.y, spi[j].y));
                }

                // 判断三角形朝向
                vec4f v01 = cpf[1] - cpf[0];
                vec4f v02 = cpf[2] - cpf[0];
                vec4f normal = vec_cross(v01, v02);

                // 左手坐标系Z轴朝相机观察方向，z > 0说明与相机朝向方向相同所以舍去不绘制
                if (normal.z > 0.0f) continue;

                for (int x = box_min.x; x < box_max.x; x++) {
                    for (int y = box_min.y; y < box_max.y; y++) {
                        // 屏幕重心坐标比例
                        vec3f bc_s = math::barycentric(spi, vec2i(x, y));
                        if (bc_s.x < 0 || bc_s.y < 0 || bc_s.z < 0) continue;

                        // 屏幕坐标系中的重心坐标无法为透视变换前的顶点坐标组插值。因为透视变换破坏了深度Z的线性关系
                        // 所以要将屏幕的重心坐标比例转换到齐次坐标系得重心坐标比例
                        vec3f bc_c(bc_s.x / cpf[0].w, bc_s.y / cpf[1].w, bc_s.z / cpf[2].w);
                        bc_c = bc_c / (bc_c.x + bc_c.y + bc_c.z);
                        float z_depth = bc_c.x * cpf[0].z + bc_c.y * cpf[1].z + bc_c.z * cpf[2].z;
                        // 左手坐标系Z轴朝相机观察方向，深度由小变大（与OpenGL相反）
                        if (z_depth > z_buffer[x + y * width]) continue;
                        z_buffer[x + y * width] = z_depth;

                        color color;
                        obj.mesh.shader->frag(color);
                        draw_pixel(x, y, color);
                    }
                }
            }
        }
    }
            render;
}

#endif //SHEEPRENDER_SR_RENDER_H
