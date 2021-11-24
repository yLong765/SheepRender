//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_SHADER_H
#define SHEEPRENDER_SR_SHADER_H

namespace SR {

    enum SHADER_TYPE {
        CONSTANT_SHADER,
        PHONG_SHADER,
    };

    typedef struct sr_shader {
        mat4x4f mat_model;
        mat4x4f mat_view;
        mat4x4f mat_proj;
        mat4x4f mat_mvp;

        struct vert_in {
            vec3f vertex;
            vec3f normal;
        };

        struct frag_in {

        };

        sr_shader() = default;

        virtual vec4f vert(vert_in in) = 0;

        virtual bool frag(sr_color &color) = 0;
    } shader;
}

#endif //SHEEPRENDER_SR_SHADER_H
