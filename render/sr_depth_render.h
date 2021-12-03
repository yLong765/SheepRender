//
// Created by long on 2021/12/3.
//

#ifndef SHEEPRENDER_SR_DEPTH_RENDER_H
#define SHEEPRENDER_SR_DEPTH_RENDER_H

namespace SR {
    typedef struct sr_depth_render : public sr_mesh_render {
        mat4x4f mvp;

        sr_depth_render(sr_texture_2d *texture, sr_camera *camera, sr_light *light, float *z_buffer) : sr_mesh_render(
                texture, camera, light, z_buffer) {}

        // 初始化MVP矩阵
        void init(sr_object *obj) override {
            mvp = obj->transform.get_world_matrix() * camera->get_look_at_matrix() *
                  camera->get_perspective_matrix(math::aspect(width, height));
        }

        void vertex(int vert_id, vert_in in) override {
            ndc_p[vert_id] = clip2ndc(in.v3f[VERTEX_MODEL].xyz1() * mvp);
            screen_pf[vert_id] = ndc2screen(ndc_p[vert_id]);
            screen_pi[vert_id] = screen_f2i(screen_pf[vert_id]);
            box_bound(box_min, box_max, screen_pi[vert_id]);
        }

        // 基于重心坐标
        void fragment() override {
            for (int x = box_min.x; x < box_max.x; x++) {
                for (int y = box_min.y; y < box_max.y; y++) {
                    vec3f bc_s = math::barycentric(screen_pi, vec2i(x, y));
                    if (bc_s.x < 0 || bc_s.y < 0 || bc_s.z < 0) continue;
                    vec3f prop = get_prop(bc_s);
                    float z_depth = get_depth(prop);
                    if (z_depth > z_buffer[x + y * width]) continue;
                    z_buffer[x + y * width] = z_depth;
                    color color(1, 1, 1, 1);
                    color *= z_depth;
                    draw_pixel(x, y, color);
                }
            }
        }
    } depth_render;
}

#endif //SHEEPRENDER_SR_DEPTH_RENDER_H
