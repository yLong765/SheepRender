//
// Created by long on 2021/12/2.
//

#ifndef SHEEPRENDER_SR_OBJECT_RENDER_H
#define SHEEPRENDER_SR_OBJECT_RENDER_H

namespace SR {
    typedef struct sr_object_render : public sr_render {
    protected:
        vec4f ndc_p[3];     // 归一化设备坐标
        vec2f screen_pf[3]; // 屏幕坐标
        vec2i screen_pi[3]; // 整数屏幕坐标
        vert_out out[3];    // 顶点输出
        vec2i box_min;
        vec2i box_max;
        sr_shader *shader;
    public:
        sr_object_render(sr_texture_2d *texture, sr_camera *camera, sr_light *light) : sr_render(texture, camera,
                                                                                                 light) {}

        // shader上下文初始化
        void shader_context_init(sr_object *obj) {
            shader->mat_model = obj->transform.get_world_matrix();
            shader->mat_view = camera->get_look_at_matrix();
            shader->mat_proj = camera->get_perspective_matrix(math::aspect(width, height));
            shader->light.color = light->color.c;
            shader->light.direction = light->direction;
            shader->light.position = light->position;
            shader->view_pos = camera->from;
        }

        // 判断三角形朝向
        bool triangles_direction() {
            vec4f v01 = ndc_p[1] - ndc_p[0];
            vec4f v02 = ndc_p[2] - ndc_p[0];
            vec4f normal = vec_cross(v01, v02);

            // 左手坐标系Z轴朝相机观察方向，z > 0说明与相机朝向方向相同所以舍去不绘制
            if (normal.z > 0.0f) return false;
            return true;
        }

        // 获取插值值
        vec3f get_prop(vec3f bc_s) {
            vec3f bc_ndc(bc_s.x / ndc_p[0].w, bc_s.y / ndc_p[1].w, bc_s.z / ndc_p[2].w);
            bc_ndc = bc_ndc / (bc_ndc.x + bc_ndc.y + bc_ndc.z);
            return bc_ndc;
        }

        // 获取深度值
        float get_depth(vec3f prop) {
            return prop.x * ndc_p[0].z + prop.y * ndc_p[1].z + prop.z * ndc_p[2].z;
        }

        // 顶点计算
        void vertex(int id, int vert_id, sr_object *obj) override {
            vert_in in;
            in.v3f[VERTEX_MODEL] = obj->mesh->vertices[id];
            in.v3f[NORMAL_MODEL] = obj->mesh->normals[id];
            out[vert_id] = shader->vert(in);
            ndc_p[vert_id] = to_ndc(out[vert_id].v4f[VERTEX_CLIP]);
            screen_pf[vert_id] = to_screen_f(ndc_p[vert_id]);
            screen_pi[vert_id] = to_screen_i(screen_pf[vert_id]);
            box_bound(box_min, box_max, screen_pi[vert_id]);
        }

        // 像素计算
        void fragment() override {
            for (int x = box_min.x; x < box_max.x; x++) {
                for (int y = box_min.y; y < box_max.y; y++) {
                    // 屏幕重心坐标比例
                    vec3f bc_s = math::barycentric(screen_pi, vec2i(x, y));
                    if (bc_s.x < 0 || bc_s.y < 0 || bc_s.z < 0) continue;

                    // 屏幕坐标系中的重心坐标无法为透视变换前的顶点坐标组插值。因为透视变换破坏了深度Z的线性关系
                    // 所以要将屏幕的重心坐标比例转换到齐次坐标系得重心坐标比例
                    vec3f prop = get_prop(bc_s);
                    float z_depth = get_depth(prop);
                    // 左手坐标系Z轴朝相机观察方向，深度由小变大（与OpenGL相反）
                    if (z_depth > z_buffer[x + y * width]) continue;
                    z_buffer[x + y * width] = z_depth;

                    // 插值
                    frag_in in = interpolation(out, prop);

                    color color;
                    if (!shader->frag(in, color)) continue;
                    draw_pixel(x, y, color);
                }
            }
        }

        // 绘制模型
        // 基于重心坐标的方法渲染（慢）
        void draw() override {
            std::vector<sr_object *> objs = object_mgr::get_objs(); // 临时
            for (auto &obj : objs) {
                shader = obj->mesh->shader;
                shader_context_init(obj);
                for (int i = 0; i < obj->mesh->triangles.size(); i += 3) {
                    for (int j = 0; j < 3; j++) {
                        vertex(obj->mesh->triangles[i + j], j, obj);
                    }
                    if (!triangles_direction()) continue;
                    fragment();
                }
            }
        }
    } object_render;
}

#endif //SHEEPRENDER_SR_OBJECT_RENDER_H
