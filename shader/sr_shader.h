//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_SHADER_H
#define SHEEPRENDER_SR_SHADER_H

namespace SR {

    enum shader_type {
        PHONG,
    };

    typedef struct sr_shader {
        mat4x4 world;
        mat4x4 view;
        mat4x4 projection;
        mat4x4 trans;

        sr_shader() = default;

        virtual vec4 vert(vec4 vertex, vec4 normal) = 0;

        virtual bool frag(sr_color &color) = 0;
    } shader;
}

#endif //SHEEPRENDER_SR_SHADER_H
