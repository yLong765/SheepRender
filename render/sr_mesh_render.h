//
// Created by long on 2021/12/3.
//

#ifndef SHEEPRENDER_SR_MESH_RENDER_H
#define SHEEPRENDER_SR_MESH_RENDER_H

namespace SR {
    typedef struct sr_mesh_render : public sr_base_render {
    protected:
        sr_light *light;    // 相对于哪个灯光渲染

        vec4f ndc_p[3];     // 归一化设备坐标
        vec2f screen_pf[3]; // 屏幕坐标
        vec2i screen_pi[3]; // 整数屏幕坐标
        vert_out out[3];    // 顶点输出
        vec2i box_min;
        vec2i box_max;
        sr_shader *shader;

        sr_mesh_render(sr_texture_2d *texture, sr_camera *camera, sr_light *light, float *z_buffer) : sr_base_render(
                texture, camera, z_buffer) {
            this->light = light;
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

        // 三角形外接矩形
        void box_bound(vec2i &box_min, vec2i &box_max, vec2i screen_pi) const {
            box_min.x = std::max(0, std::min(box_min.x, screen_pi.x));
            box_min.y = std::max(0, std::min(box_min.y, screen_pi.y));
            box_max.x = std::min(width - 1, std::max(box_max.x, screen_pi.x));
            box_max.y = std::min(height - 1, std::max(box_max.y, screen_pi.y));
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

        // shader上下文初始化
        virtual void init(sr_object *obj) = 0;

        // 顶点计算
        virtual void vertex(int vert_id, vert_in in) = 0;

        // 像素计算
        virtual void fragment() = 0;

    public:
        virtual void draw() {
            for (auto &obj : object_mgr::instance().get_objs()) {
                init(obj);
                for (int i = 0; i < obj->get_mesh()->triangles.size(); i += 3) {
                    for (int j = 0; j < 3; j++) {
                        sr_mesh *mesh = obj->get_mesh();
                        int id = mesh->triangles[i + j];
                        vert_in in;
                        in.v3f[VERTEX_MODEL] = mesh->vertices[id];
                        in.v3f[NORMAL_MODEL] = mesh->normals[id];
                        vertex(j, in);
                    }
                    if (!triangles_direction()) continue;
                    fragment();
                }
            }
        };
    } mesh_render;
}

#endif //SHEEPRENDER_SR_MESH_RENDER_H
