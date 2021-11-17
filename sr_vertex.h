//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_VERTEX_H
#define SHEEPRENDER_SR_VERTEX_H

namespace SR {
    typedef struct sr_vertex {
        vec3 point;
        sr_texcoord texcoord;
        sr_color color;

        sr_vertex() = default;

        sr_vertex(vec3 point, sr_texcoord texcoord, sr_color color) : point(point), texcoord(texcoord), color(color) {}
    } vertex;
}

#endif //SHEEPRENDER_SR_VERTEX_H
