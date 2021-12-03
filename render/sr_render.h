//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_RENDER_H
#define SHEEPRENDER_SR_RENDER_H

namespace SR {
    enum RENDER_TYPE {
        BARYCENTRIC_COORDINATE = 0,
        EDGE_EQUATION = 1,
        EDGE_WALKING = 2,
        // 深度渲染
        DEPTH_RENDER = 10,
    };

    struct sr_object_render;

    // 渲染类
    typedef struct sr_render {
    protected:
        int width;              // 宽
        int height;             // 高
        sr_texture_2d *texture; // 颜色写入的贴图
        sr_camera *camera;      // 相对于哪个相机渲染
        sr_light *light;        // 相对于哪个灯光渲染
        float *z_buffer;        // 深度缓冲

    public:
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
        void draw_pixel(int x, int y, color color) const {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                texture->set(x, y, color);
            }
        }

        // 绘制线
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

        // 绘制线
        void draw_line(vec4f v1, vec4f v2, color color) const {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
        }

        // 绘制三角形线框
        void draw_triangle_wireframe(vec4f v1, vec4f v2, vec4f v3, color color) const {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
            draw_line((int) v2.x, (int) v2.y, (int) v3.x, (int) v3.y, color);
            draw_line((int) v3.x, (int) v3.y, (int) v1.x, (int) v1.y, color);
        }

        // 绘制三角形线框
        void draw_wireframe(sr_object obj, sr_color color) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix(math::aspect(width, height));
            mat4x4f mvp = model * view * projection;
            for (int i = 0; i < obj.mesh->triangles.size(); i += 3) {
                vec4f screen_point[3];
                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh->triangles[i + j];
                    screen_point[j] = camera->homogenize(obj.mesh->vertices[id].xyz1() * mvp, width, height);
                }
                draw_triangle_wireframe(screen_point[0], screen_point[1], screen_point[2], color);
            }
        }

        // 绘制法线（有法线的模型）
        void draw_normal(sr_object obj, sr_color color) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix(math::aspect(width, height));
            mat4x4f mvp = model * view * projection;
            for (int i = 0; i < obj.mesh->triangles.size(); i += 3) {
                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh->triangles[i + j];
                    vec4f screen_point = camera->homogenize(obj.mesh->vertices[id].xyz1() * mvp, width, height);
                    vec4f screen_normal = camera->homogenize(
                            (obj.mesh->vertices[id] + obj.mesh->normals[id]).xyz1() * mvp, width, height);
                    draw_line(screen_point, screen_normal, color);
                }
            }
        }

        // 绘制法线（无法线的模型，并用叉乘创建垂直与屏幕的法线（此法线渲染的模型都为硬边））
        void draw_js_normal(sr_object obj, sr_color color) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix(math::aspect(width, height));
            mat4x4f mvp = model * view * projection;
            for (int i = 0; i < obj.mesh->triangles.size(); i += 3) {
                vec4f mpf[3];
                vec4f cpf[3];
                vec4f spf[3];
                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh->triangles[i + j];
                    mpf[j] = obj.mesh->vertices[id].xyz1();
                    cpf[j] = mpf[j] * mvp;
                    spf[j] = camera->homogenize(cpf[j], width, height);
                }
                vec4f v01 = mpf[1] - mpf[0];
                vec4f v02 = mpf[2] - mpf[0];
                vec4f normal = vec_normalize(vec_cross(v01, v02));
                vec4f npf[3] = {mpf[0] + normal, mpf[1] + normal, mpf[2] + normal};
                for (int j = 0; j < 3; j++) {
                    npf[j] = npf[j] * mvp;
                    npf[j] = camera->homogenize(npf[j], width, height);
                    draw_line(spf[j], npf[j], color);
                }
            }
        }

        // 绘制物体的本地坐标轴
        void draw_axis(sr_object obj) const {
            mat4x4f model = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix(math::aspect(width, height));
            // z forward
            vec4f p = obj.transform.position.xyz1();
            vec4f fp = vec4f(0, 0, 3, 1) * model + p;
            p = p * view * projection;
            fp = fp * view * projection;
            p = camera->homogenize(p, width, height);
            fp = camera->homogenize(fp, width, height);
            draw_line(p, fp, color(0, 0, 1));
            // y up
            p = obj.transform.position.xyz1();
            fp = vec4f(0, 3, 0, 1) * model + p;
            p = p * view * projection;
            fp = fp * view * projection;
            p = camera->homogenize(p, width, height);
            fp = camera->homogenize(fp, width, height);
            draw_line(p, fp, color(0, 1, 0));
            // x right
            p = obj.transform.position.xyz1();
            fp = vec4f(3, 0, 0, 1) * model + p;
            p = p * view * projection;
            fp = fp * view * projection;
            p = camera->homogenize(p, width, height);
            fp = camera->homogenize(fp, width, height);
            draw_line(p, fp, color(1, 0, 0));
        }

        // 顶点数据插值获得像素数据
        frag_in interpolation(vert_out out[3], vec3f prop) {
            frag_in in;

            for (auto &it : out[0].v1f) {
                SHADER_KEY_TYPE key = it.first;
                in.v1f[key] = prop.x * out[0].v1f[key] + prop.y * out[1].v1f[key] + prop.z * out[2].v1f[key];
            }

            for (auto &it : out[0].v2f) {
                SHADER_KEY_TYPE key = it.first;
                in.v2f[key] = prop.x * out[0].v2f[key] + prop.y * out[1].v2f[key] + prop.z * out[2].v2f[key];
            }

            for (auto &it : out[0].v3f) {
                SHADER_KEY_TYPE key = it.first;
                in.v3f[key] = prop.x * out[0].v3f[key] + prop.y * out[1].v3f[key] + prop.z * out[2].v3f[key];
            }

            for (auto &it : out[0].v4f) {
                SHADER_KEY_TYPE key = it.first;
                in.v4f[key] = prop.x * out[0].v4f[key] + prop.y * out[1].v4f[key] + prop.z * out[2].v4f[key];
            }

            return in;
        }

        // 归一化齐次坐标
        vec4f to_ndc(vec4f clip_p) {
            // 1 / w
            float rhw = 1.0f / clip_p.w;
            // 归一化到[-1, 1] cvv
            return clip_p * rhw;
        }

        // 屏幕坐标
        vec2f to_screen_f(vec4f ndc_p) {
            return {(ndc_p.x + 1.0f) * width * 0.5f, (1.0f - ndc_p.y) * height * 0.5f};
        }

        // 整数屏幕坐标：加0.5的偏移取屏幕像素方格中心对齐
        vec2i to_screen_i(vec2f screen_pf) {
            return {(int) (screen_pf.x + 0.5f), (int) (screen_pf.y + 0.5f)};
        }

        // 三角形外接矩形
        void box_bound(vec2i &box_min, vec2i &box_max, vec2i screen_pi) const {
            box_min.x = std::max(0, std::min(box_min.x, screen_pi.x));
            box_min.y = std::max(0, std::min(box_min.y, screen_pi.y));
            box_max.x = std::min(width - 1, std::max(box_max.x, screen_pi.x));
            box_max.y = std::min(height - 1, std::max(box_max.y, screen_pi.y));
        }

        // 绘制模型
        // 基于重心坐标的方法渲染（慢）
        void draw_obj_barycentric_coordinate(sr_object obj) {
            shader *shader = obj.mesh->shader;
            shader->mat_model = obj.transform.get_world_matrix();
            shader->mat_view = camera->get_look_at_matrix();
            shader->mat_proj = camera->get_perspective_matrix(math::aspect(width, height));
            shader->light.color = light->color.c;
            shader->light.direction = light->direction;
            shader->light.position = light->position;
            shader->view_pos = camera->from;
            for (int i = 0; i < obj.mesh->triangles.size(); i += 3) {
                vec4f ndc_p[3];     // 归一化设备坐标
                vec2f screen_pf[3]; // 屏幕坐标
                vec2i screen_pi[3]; // 整数屏幕坐标
                vert_out out[3];    // 顶点输出

                vec2i box_min(width - 1, height - 1);
                vec2i box_max(0, 0);

                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh->triangles[i + j];
                    vert_in in;
                    in.v3f[VERTEX_MODEL] = obj.mesh->vertices[id];
                    in.v3f[NORMAL_MODEL] = obj.mesh->normals[id];
                    out[j] = shader->vert(in);
                    ndc_p[j] = to_ndc(out[j].v4f[VERTEX_CLIP]);
                    screen_pf[j] = to_screen_f(ndc_p[j]);
                    screen_pi[j] = to_screen_i(screen_pf[j]);
                    box_bound(box_min, box_max, screen_pi[j]);
                }

                // 判断三角形朝向
                vec4f v01 = ndc_p[1] - ndc_p[0];
                vec4f v02 = ndc_p[2] - ndc_p[0];
                vec4f normal = vec_cross(v01, v02);

                // 左手坐标系Z轴朝相机观察方向，z > 0说明与相机朝向方向相同所以舍去不绘制
                if (normal.z > 0.0f) continue;

                for (int x = box_min.x; x < box_max.x; x++) {
                    for (int y = box_min.y; y < box_max.y; y++) {
                        // 屏幕重心坐标比例
                        vec3f bc_s = math::barycentric(screen_pi, vec2i(x, y));
                        if (bc_s.x < 0 || bc_s.y < 0 || bc_s.z < 0) continue;

                        // 屏幕坐标系中的重心坐标无法为透视变换前的顶点坐标组插值。因为透视变换破坏了深度Z的线性关系
                        // 所以要将屏幕的重心坐标比例转换到齐次坐标系得重心坐标比例
                        vec3f bc_c(bc_s.x / ndc_p[0].w, bc_s.y / ndc_p[1].w, bc_s.z / ndc_p[2].w);
                        bc_c = bc_c / (bc_c.x + bc_c.y + bc_c.z);
                        float z_depth = bc_c.x * ndc_p[0].z + bc_c.y * ndc_p[1].z + bc_c.z * ndc_p[2].z;
                        // 左手坐标系Z轴朝相机观察方向，深度由小变大（与OpenGL相反）
                        if (z_depth > z_buffer[x + y * width]) continue;
                        z_buffer[x + y * width] = z_depth;

                        // 插值
                        frag_in in = interpolation(out, bc_c);

                        color color;
                        if (!shader->frag(in, color)) continue;
                        draw_pixel(x, y, color);
                    }
                }
            }
        }

        void draw_depth(sr_object obj) {
            mat4x4f mvp = obj.transform.get_world_matrix() * camera->get_look_at_matrix() *
                          camera->get_perspective_matrix(math::aspect(width, height));
            for (int i = 0; i < obj.mesh->triangles.size(); i += 3) {
                vec4f cpf[3];       // 齐次空间坐标
                vec2f spf[3];       // 屏幕坐标
                vec2i spi[3];       // 整数屏幕坐标

                vec2i box_min(width - 1, height - 1);
                vec2i box_max(0, 0);

                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh->triangles[i + j];
                    cpf[j] = obj.mesh->vertices[id].xyz1() * mvp;

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

                        color color(1, 1, 1, 1);
                        color *= z_depth;
                        draw_pixel(x, y, color);
                    }
                }
            }
        }

        virtual void draw() = 0;

        virtual void vertex(int id, int j, sr_object *obj) = 0;

        virtual void fragment() = 0;
    } render;
}

#endif //SHEEPRENDER_SR_RENDER_H