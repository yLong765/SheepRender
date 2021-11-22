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
        mat4x4f mat_model;
        mat4x4f mat_view;
        mat4x4f mat_proj;
        mat4x4f mat_mvp;

        sr_shader() = default;

        virtual vec4f vert(vec4f vertex, vec4f normal) = 0;

        virtual bool frag(sr_color &color) = 0;
    } shader;
}

#endif //SHEEPRENDER_SR_SHADER_H
