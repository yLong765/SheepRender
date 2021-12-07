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

    // 渲染类
    typedef struct sr_render : public sr_singleton<sr_render> {
    private:
        sr_texture_2d *texture; // 颜色写入的贴图
        sr_camera *camera;      // 相对于哪个相机渲染
        sr_light *light;        // 相对于哪个灯光渲染
        float *z_buffer;        // 深度缓冲

        /// 线框渲染器
        sr_color wireframe_color = sr_color(1.0f, 1.0f, 1.0f);
        sr_wireframe_render *wireframe_render = nullptr;  // 线框绘制

        sr_wireframe_render *get_wireframe_render() {
            if (wireframe_render == nullptr) {
                wireframe_render = new sr_wireframe_render(texture, camera, z_buffer);
            }
            return wireframe_render;
        }
        /// 线框渲染器

        /// 网格渲染器
        sr_barycentric_render *barycentric_render = nullptr;

        sr_barycentric_render *get_barycentric_render() {
            if (barycentric_render == nullptr) {
                barycentric_render = new sr_barycentric_render(texture, camera, light, z_buffer);
            }
            return barycentric_render;
        }

        sr_depth_render *depth_render = nullptr;

        sr_depth_render *get_depth_render() {
            if (depth_render == nullptr) {
                depth_render = new sr_depth_render(texture, camera, light, z_buffer);
            }
            return depth_render;
        }
        /// 网格渲染器

    public:
        sr_render() = default;

        void init(sr_texture_2d *texture, sr_camera *camera, sr_light *light) {
            this->texture = texture;
            this->camera = camera;
            this->light = light;
            this->z_buffer = (float *) malloc(sizeof(float) * texture->width * texture->height);
        }

        // 设置贴图
        void set_texture(sr_texture_2d *texture) {
            this->texture = texture;
            this->z_buffer = (float *) malloc(sizeof(float) * texture->width * texture->height);
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
        void clear_color(color color = sr_color(0.0f, 0.0f, 0.0f)) const {
            for (int i = 0; i < texture->get_count(); i++) {
                texture->set(i, color);
            }
        }

        // 清空深度缓冲
        void clear_z_buffer() const {
            for (int i = 0; i < texture->get_count(); i++) {
                z_buffer[i] = FLT_MAX;
            }
        }

        void clear() const {
            clear_z_buffer();
            clear_color();
        }

        void draw_mesh(RENDER_MESH_TYPE type = BARYCENTRIC_COORDINATE) {
            sr_mesh_render *render = nullptr;
            switch (type) {
                case BARYCENTRIC_COORDINATE:
                    render = get_barycentric_render();
                    break;
                case EDGE_EQUATION:
                    break;
                case EDGE_WALKING:
                    break;
                case DEPTH_RENDER:
                    render = get_depth_render();
                    break;
            }
            if (render != nullptr) {
                render->draw();
            }
        }

        void set_draw_color(sr_color color) {
            wireframe_color = color;
        }

        void draw_line(vec3f start, vec3f end) {
            get_wireframe_render()->draw_world_line(start, end, wireframe_color);
        }

        void draw_ray(vec3f from, vec3f direction) {
            get_wireframe_render()->draw_world_ray(from, direction, wireframe_color);
        }

        void draw_axis(vec3f origin, vec3f up, vec3f right, vec3f forward) {
            get_wireframe_render()->draw_world_axis(origin, up, right, forward);
        }

        void draw_axis(sr_object *obj) {
            get_wireframe_render()->draw_world_axis(obj->get_trans()->position, obj->get_trans()->up(),
                                                    obj->get_trans()->right(), obj->get_trans()->forward());
        }
    } render;
}

#endif //SHEEPRENDER_SR_RENDER_H