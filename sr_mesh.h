//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_MESH_H
#define SHEEPRENDER_SR_MESH_H

namespace SR {
    typedef struct sr_mesh {
        std::vector<vertex> vertices;
        std::vector<int> triangles;
        std::vector<vec3> normals;

        sr_mesh() = default;
    } mesh;
}

#endif //SHEEPRENDER_SR_MESH_H
