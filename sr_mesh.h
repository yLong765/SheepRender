//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_MESH_H
#define SHEEPRENDER_SR_MESH_H

namespace SR {
    // 模型网格
    typedef struct sr_mesh {
        std::vector<vec3f> vertices;    // 顶点数组
        std::vector<vec3f> normals;     // 顶点向量数组
        std::vector<vec2f> uv;          // 顶点贴图UV
        std::vector<int> triangles;     // 三角形组合
        sr_shader *shader;              // 所使用shader

        sr_mesh() {
            shader = new sr_constant_shader();
        };

        // 设置shader
        void set_shader(SHADER_TYPE type) {
            switch (type) {
                case CONSTANT_SHADER:
                    shader = new sr_constant_shader();
                    break;
                case PHONG_SHADER:
                    shader = new sr_phong_shader();
                    break;
            }
        }
    } mesh;
}

#endif //SHEEPRENDER_SR_MESH_H
