//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_PHONG_SHADER_H
#define SHEEPRENDER_SR_PHONG_SHADER_H

namespace SR {
    typedef struct sr_phong_shader : public sr_shader {
        sr_phong_shader() = default;

        vec4 vert(vec4 vertex) override {
            return vertex;
        }

        bool frag() override {
            return true;
        }
    } phong_shader;
}

#endif //SHEEPRENDER_SR_PHONG_SHADER_H
