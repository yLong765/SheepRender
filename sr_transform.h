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

        mat4x4 scale_mat;
        mat4x4 rotation_mat;
        mat4x4 translation_mat;

        mat4x4 world;
        mat4x4 view;

        sr_transform() {
            position = vec3::zero();
            euler = vec3::zero();
            scale = vec3::one();
            scale_mat = mat4x4::identity();
            rotation_mat = mat4x4::identity();
            translation_mat = mat4x4::identity();
            world = mat4x4::identity();
        }

        void update() {
            scale_mat = mat4x4::get_scale(scale);
            rotation_mat = mat4x4::identity();
            rotation_mat = rotation_mat * mat4x4::get_z_rotation(deg2rad(euler.z));
            rotation_mat = rotation_mat * mat4x4::get_y_rotation(deg2rad(euler.y));
            rotation_mat = rotation_mat * mat4x4::get_x_rotation(deg2rad(euler.x));
            translation_mat = mat4x4::get_translation(position);
            world = scale_mat * rotation_mat * translation_mat;
        }

        mat4x4 get_world_matrix() {
            scale_mat = mat4x4::get_scale(scale);
            rotation_mat = mat4x4::identity();
            rotation_mat = rotation_mat * mat4x4::get_z_rotation(deg2rad(euler.z));
            rotation_mat = rotation_mat * mat4x4::get_y_rotation(deg2rad(euler.y));
            rotation_mat = rotation_mat * mat4x4::get_x_rotation(deg2rad(euler.x));
            translation_mat = mat4x4::get_translation(position);
            return scale_mat * rotation_mat * translation_mat;
        }
    } transform;
}

#endif //SHEEPRENDER_SR_TRANSFORM_H
