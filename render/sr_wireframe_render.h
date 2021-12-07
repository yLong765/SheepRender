//
// Created by long on 2021/12/7.
//

#ifndef SHEEPRENDER_SR_WIREFRAME_RENDER_H
#define SHEEPRENDER_SR_WIREFRAME_RENDER_H

namespace SR {
    typedef struct sr_wireframe_render : public sr_base_render {
        sr_wireframe_render(sr_texture_2d *texture, sr_camera *camera, float *z_buffer) : sr_base_render(texture,
                                                                                                         camera,
                                                                                                         z_buffer) {}

        // 绘制世界坐标线段
        void draw_world_line(vec3f start, vec3f end, sr_color color) {
            mat4x4f view = get_view_matrix();
            mat4x4f projection = get_projection_matrix();
            vec2i s_start = clip2screen_i(start.xyz1() * view * projection);
            vec2i s_end = clip2screen_i(end.xyz1() * view * projection);
            draw_line(s_start, s_end, color);
        }

        // 绘制世界坐标射线
        void draw_world_ray(vec3f from, vec3f direction, sr_color color) {
            mat4x4f view = get_view_matrix();
            mat4x4f projection = get_projection_matrix();
            vec2i s_from = clip2screen_i(from.xyz1() * view * projection);
            vec2i s_end = clip2screen_i((from + direction).xyz1() * view * projection);
            draw_line(s_from, s_end, color);
        }

        // 绘制世界坐标物体的本地坐标轴
        void draw_world_axis(vec3f origin, vec3f up, vec3f right, vec3f forward) {
            mat4x4f view = get_view_matrix();
            mat4x4f projection = get_projection_matrix();
            vec2i s_origin = clip2screen_i(origin.xyz1() * view * projection);
            vec2i s_up = clip2screen_i((origin + up).xyz1() * view * projection);
            vec2i s_right = clip2screen_i((origin + right).xyz1() * view * projection);
            vec2i s_forward = clip2screen_i((origin + forward).xyz1() * view * projection);
            draw_line(s_origin, s_up, sr_color(0.0f, 1.0f, 0.0f));
            draw_line(s_origin, s_right, sr_color(0.0f, 0.0f, 1.0f));
            draw_line(s_origin, s_forward, sr_color(1.0f, 0.0f, 0.0f));
        }
    } wireframe_render;
}

#endif //SHEEPRENDER_SR_WIREFRAME_RENDER_H
