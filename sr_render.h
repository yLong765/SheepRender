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

        void draw_triangle_wireframe(vec4f v1, vec4f v2, vec4f v3, color color) const {
            draw_line((int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
            draw_line((int) v2.x, (int) v2.y, (int) v3.x, (int) v3.y, color);
            draw_line((int) v3.x, (int) v3.y, (int) v1.x, (int) v1.y, color);
        }

        void draw_wireframe(sr_object obj, sr_color color) const {
            mat4x4f world = obj.transform.get_world_matrix();
            mat4x4f view = camera->get_look_at_matrix();
            mat4x4f projection = camera->get_perspective_matrix();
            mat4x4f trans = world * view * projection;
            for (int i = 0; i < obj.mesh.triangles.size(); i += 3) {
                vec4f screen_point[3];
                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh.triangles[i + j];
                    screen_point[j] = camera->homogenize(obj.mesh.vertices[id].xyz1() * trans);
                }
                draw_triangle_wireframe(screen_point[0], screen_point[1], screen_point[2], color);
            }
        }

        void draw(sr_object obj) const {
            obj.mesh.shader->world = obj.transform.get_world_matrix();
            obj.mesh.shader->view = camera->get_look_at_matrix();
            obj.mesh.shader->projection = camera->get_perspective_matrix();
            for (int i = 0; i < obj.mesh.triangles.size(); i += 3) {
                vec4f clip_p[3];
                vec2f screen_p[3];

                vec2f screen_clamp(width - 1, height - 1);
                vec2f boxmin(screen_clamp);
                vec2f boxmax(0, 0);

                for (int j = 0; j < 3; j++) {
                    int id = obj.mesh.triangles[i + j];
                    clip_p[j] = obj.mesh.shader->vert(obj.mesh.vertices[id].xyz1(), obj.mesh.normals[id].xyz1());
                    vec4f sp = camera->homogenize(clip_p[j]);
                    screen_p[j] = vec2f(sp.x, sp.y);

                    boxmin.x = std::max(0.0f, std::min(boxmin.x, screen_p[j].x));
                    boxmin.y = std::max(0.0f, std::min(boxmin.y, screen_p[j].y));
                    boxmax.x = std::min(screen_clamp.x, std::max(boxmax.x, screen_p[j].x));
                    boxmax.y = std::min(screen_clamp.y, std::max(boxmax.y, screen_p[j].y));
                }

                for (int x = (int) boxmin.x; x < (int) boxmax.x; x++) {
                    for (int y = (int) boxmin.y; y < (int) boxmax.y; y++) {
                        vec3f bc_screen = math::barycentric(screen_p[0], screen_p[1], screen_p[2], vec2f(x, y));
                        if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
                            continue;
                        }
                        color color;
                        obj.mesh.shader->frag(color);
                        draw_pixel(x, y, color);
                    }
                }
            }
        }

//        void draw_base(sr_object obj) const {
//            obj.mesh.shader->world = obj.transform.get_world_matrix();
//            obj.mesh.shader->view = camera->get_look_at_matrix();
//            obj.mesh.shader->projection = camera->get_perspective_matrix();
//            for (int i = 0; i < obj.mesh.triangles.size(); i += 3) {
//                vec4f clip_point[3];
//                vec4f screen_point[3];
//                vec2f screen_point2[3];
//                for (int j = 0; j < 3; j++) {
//                    int id = obj.mesh.triangles[i + j];
//                    clip_point[j] = obj.mesh.shader->vert(vec4f(obj.mesh.vertices[id], 1));
//                    screen_point[j] = camera->homogenize(clip_point[j]);
//                    screen_point2[j] = vec2f(screen_point[j].x / screen_point[j].w,
//                                            screen_point[j].y / screen_point[j].w);
//                }
//                vec2f boxmin(FLT_MAX, FLT_MAX);
//                vec2f boxmax(-FLT_MAX, -FLT_MAX);
//                float heightf = (float) texture->height - 1.0f;
//                for (int k = 0; k < 3; k++) {
//                    for (int j = 0; j < 2; j++) {
//                        boxmin[j] = std::max(0.0f, std::min(boxmin[j], screen_point2[i][j]));
//                        boxmax[j] = std::min(heightf, std::max(boxmax[j], screen_point2[i][j]));
//                    }
//
//                    for (int x = (int) boxmin.x; x <= boxmax.x; x++) {
//                        for (int y = (int) boxmin.y; y <= boxmax.y; y++) {
//                            vec3f bc_screen = math::barycentric(screen_point2[0], screen_point2[1], screen_point2[2],
//                                                               vec2f(x, y));
//                            vec3f bc_clip = vec3f(bc_screen.x / screen_point[0].w, bc_screen.y / screen_point[1].w,
//                                                bc_screen.z / screen_point[2].w);
//                            bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);
//                            float z_depth = vec3f(clip_point[0].z, clip_point[1].z, clip_point[2].z) * bc_clip;
//                            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0 ||
//                                z_buffer[x + y * texture->width] > z_depth)
//                                continue;
//                            color color;
//                            bool discard = obj.mesh.shader->frag(bc_clip, color);
//                            if (discard) continue;
//                            z_buffer[x + y * texture->width] = z_depth;
//                            texture->set(x, y, color);
//                        }
//                    }
//                }
//            }
//        }
    } render;
}

#endif //SHEEPRENDER_SR_RENDER_H
