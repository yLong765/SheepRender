//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_MESH_H
#define SHEEPRENDER_SR_MESH_H

namespace SR {
    typedef struct sr_mesh {
        std::vector<vec3f> vertices;
        std::vector<vec3f> normals;
        std::vector<vec2f> uv;
        std::vector<int> triangles;
        sr_shader *shader;

        sr_mesh() {
            shader = new sr_constant_shader();
        };

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
