//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_OBJECT_H
#define SHEEPRENDER_SR_OBJECT_H

namespace SR {
    // 场景物体基类
    typedef struct sr_object {
        // 模型的变换
        sr_transform transform;
        // 模型的网格
        sr_mesh mesh;

        sr_object(const char *path) {
            mesh = obj_loader::load(path);
        }

        // 设置shader
        void set_shader(SHADER_TYPE type) {
            mesh.set_shader(type);
        }
    } object;
}

#endif //SHEEPRENDER_SR_OBJECT_H
