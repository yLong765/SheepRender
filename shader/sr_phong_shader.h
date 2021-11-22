//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_PHONG_SHADER_H
#define SHEEPRENDER_SR_PHONG_SHADER_H

namespace SR {
    typedef struct sr_phong_shader : public sr_shader {
        sr_phong_shader() = default;

        vec4f n_world;

        vec4f vert(vec4f vertex, vec4f normal) override {
            n_world = vec_normalize(normal * world);
            return vertex * world * view * projection;
        }

        bool frag(sr_color &color) override {
            float intensity = vec_dot(n_world, vec4f(0, -1, 0, 1));
            if (intensity > 0) {
                color.set(255 * intensity, 255 * intensity, 255 * intensity);
            }
            return true;
        }
    } phong_shader;
}

#endif //SHEEPRENDER_SR_PHONG_SHADER_H
