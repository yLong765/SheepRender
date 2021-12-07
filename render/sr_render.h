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
        LINE_WIREFRAME = 0,
        TRIANGLE_WIREFRAME = 1,
        AXIS_WIREFRAME = 2,
    };

    // 渲染类
    typedef struct sr_render : public sr_singleton<sr_render> {
    private:
        sr_color wireframe_color = sr_color(1.0f, 1.0f, 1.0f);

        sr_wireframe_render *wireframe_render = nullptr;  // 线框绘制

        sr_wireframe_render *get_wireframe_render() {
            if (wireframe_render == nullptr) {
                wireframe_render = new sr_wireframe_render(texture, camera, z_buffer);
            }
            return wireframe_render;
        }

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
            get_wireframe_render()->draw_world_axis(obj->transform.position, obj->transform.up(),
                                                    obj->transform.right(), obj->transform.forward());
        }
    } render;
}

#endif //SHEEPRENDER_SR_RENDER_H