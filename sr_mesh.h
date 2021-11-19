//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_MESH_H
#define SHEEPRENDER_SR_MESH_H

namespace SR {
    typedef struct sr_mesh {
        std::vector<vec3> vertices;
        std::vector<vec3> normals;
        std::vector<vec2> uv;
        std::vector<int> triangles;
        sr_shader *shader;

        sr_mesh() {
            shader = new sr_phong_shader();
        };

        void set_shader(shader_type type) {
            switch (type) {
                case PHONG:
                    shader = new sr_phong_shader();
                    break;
            }
        }
    } mesh;
}

#endif //SHEEPRENDER_SR_MESH_H
