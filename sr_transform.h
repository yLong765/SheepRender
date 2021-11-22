//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_TRANSFORM_H
#define SHEEPRENDER_SR_TRANSFORM_H

namespace SR {
    typedef struct sr_transform {
        vec3f position{};
        vec3f euler{};
        vec3f scale{};

        mat4x4f world;

        bool is_static;
        bool is_init;

        sr_transform(bool is_state = false) : is_static(is_static) {
            position = vec3f::zero();
            euler = vec3f::zero();
            scale = vec3f::one();
            world = mat4x4f::identity();
            is_init = false;
        }

        void set_static(bool is_static) {
            this->is_static = is_static;
            this->is_init = false;
        }

        mat4x4f get_world_matrix() {
            if (!is_static || !is_init) {
                mat4x4f scale_mat = mat_scale(scale);
                mat4x4f rotation_mat = mat_rotation(deg2rad(euler.x), deg2rad(euler.y), deg2rad(euler.z));
                mat4x4f translation_mat = mat_translation(position);
                world = scale_mat * rotation_mat * translation_mat;
                is_init = true;
            }
            return world;
        }


    } transform;
}

#endif //SHEEPRENDER_SR_TRANSFORM_H
