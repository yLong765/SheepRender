//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_TRANSFORM_H
#define SHEEPRENDER_SR_TRANSFORM_H

namespace SR {
    typedef struct sr_transform {
        vec3 position{};
        vec3 euler{};
        vec3 scale{};

        mat4x4 world;

        bool is_static;
        bool is_init;

        sr_transform(bool is_state = false) : is_static(is_static) {
            position = vec3::zero();
            euler = vec3::zero();
            scale = vec3::one();
            world = mat4x4::identity();
            is_init = false;
        }

        void set_static(bool is_static) {
            this->is_static = is_static;
            this->is_init = false;
        }

        mat4x4 get_world_matrix() {
            if (!is_static || !is_init) {
                mat4x4 scale_mat = mat4x4::get_scale(scale);
                mat4x4 rotation_mat = mat4x4::get_rotation(deg2rad(euler.x), deg2rad(euler.y), deg2rad(euler.z));
                mat4x4 translation_mat = mat4x4::get_translation(position);
                world = scale_mat * rotation_mat * translation_mat;
                is_init = true;
            }
            return world;
        }
    } transform;
}

#endif //SHEEPRENDER_SR_TRANSFORM_H
