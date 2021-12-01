//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_TRANSFORM_H
#define SHEEPRENDER_SR_TRANSFORM_H

namespace SR {
    // 变换类
    typedef struct sr_transform {
        vec3f position{};   // 世界位置
        vec3f euler{};      // 欧拉角
        vec3f scale{};      // 缩放

        mat4x4f world;      // model转world矩阵

        bool is_static;     // 是否静态，如果静态不更新world矩阵节省运算
        bool is_init;       // 是否初始化

        sr_transform(bool is_state = false) : is_static(is_static) {
            position = vec3f::zero();
            euler = vec3f::zero();
            scale = vec3f::one();
            world = mat4x4f::identity();
            is_init = false;
        }

        // 设置是否静态
        void set_static(bool is_static) {
            this->is_static = is_static;
            this->is_init = false;
        }

        // 获取world矩阵
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

        // 获取模型前方
        vec3f forward() {
            return (vec3f::forward().xyz1() * get_world_matrix()).xyz();
        }
    } transform;
}

#endif //SHEEPRENDER_SR_TRANSFORM_H
