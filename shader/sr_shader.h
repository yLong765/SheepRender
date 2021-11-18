//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_SHADER_H
#define SHEEPRENDER_SR_SHADER_H

namespace SR {
    typedef struct sr_shader {
        sr_shader() = default;

        virtual vec4 vert() = 0;

        virtual bool frag() = 0;
    } shader;
}

#endif //SHEEPRENDER_SR_SHADER_H
