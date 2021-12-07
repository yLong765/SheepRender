//
// Created by long on 2021/12/7.
//

#ifndef SHEEPRENDER_SR_EDGE_EQUATION_RENDER_H
#define SHEEPRENDER_SR_EDGE_EQUATION_RENDER_H

namespace SR {
    typedef struct sr_edge_equation_render : public sr_mesh_render {
        sr_edge_equation_render(sr_texture_2d *texture, sr_camera *camera, sr_light *light, float *z_buffer)
                : sr_mesh_render(texture, camera, light, z_buffer) {}

        // shader上下文初始化
        void init(sr_object *obj) override {
            shader = obj->get_mesh()->shader;
            shader->mat_model = obj->get_trans()->get_world_matrix();
            shader->mat_view = get_view_matrix();
            shader->mat_proj = get_projection_matrix();
            shader->light.color = light->color.c;
            shader->light.direction = light->direction;
            shader->light.position = light->position;
            shader->view_pos = camera->from;
        }

        // 顶点计算
        void vertex(int vert_id, vert_in in) override {
            out[vert_id] = shader->vert(in);
            ndc_p[vert_id] = clip2ndc(out[vert_id].v4f[VERTEX_CLIP]);
            screen_pf[vert_id] = ndc2screen(ndc_p[vert_id]);
            screen_pi[vert_id] = screen_f2i(screen_pf[vert_id]);
            box_bound(box_min, box_max, screen_pi[vert_id]);
        }

        // 像素计算(基于edge-equation)(慢)
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
    } edge_equation_render;
}

#endif //SHEEPRENDER_SR_EDGE_EQUATION_RENDER_H
