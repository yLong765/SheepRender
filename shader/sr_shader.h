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

    enum SHADER_KEY_TYPE {
        VERTEX_MODEL = 0,
        VERTEX_CLIP = 1,
        NORMAL_MODEL = 2,
        NORMAL_WORLD = 3,
        VIEW_WORLD = 4,
    };

    typedef struct shader_context {
        std::map<SHADER_KEY_TYPE, float> f;
        std::map<SHADER_KEY_TYPE, vec2f> v2f;
        std::map<SHADER_KEY_TYPE, vec3f> v3f;
        std::map<SHADER_KEY_TYPE, vec4f> v4f;
    } vert_in, vert_out, frag_in;

    struct vary_context {
        shader_context context[3];

        shader_context &operator[](const size_t i) {
            assert(i < 3);
            return context[i];
        }

        shader_context operator[](const size_t i) const {
            assert(i < 3);
            return context[i];
        }
    };

    typedef struct sr_shader {
        mat4x4f mat_model;
        mat4x4f mat_view;
        mat4x4f mat_proj;
        vec3f view_pos;

        struct Light {
            vec3f color;
            vec3f direction;
            vec3f position;
        } light;

        sr_shader() = default;

        mat4x4f get_mvp() const { return mat_model * mat_view * mat_proj; }

        virtual vert_out vert(vert_in in) = 0;

        virtual bool frag(frag_in in, sr_color &color) = 0;
    } shader;
}

#endif //SHEEPRENDER_SR_SHADER_H
