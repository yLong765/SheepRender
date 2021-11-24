//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_PHONG_SHADER_H
#define SHEEPRENDER_SR_PHONG_SHADER_H

namespace SR {
    typedef struct sr_phong_shader : public sr_shader {
        sr_phong_shader() = default;

        vec3f n_world;
        float intensity;

        vec4f vert(vert_in in) override {
            n_world = vec_normalize(in.normal * mat_invert(mat_model).transpose());
            intensity = vec_dot(n_world, vec3f(0, 1, 0));
            intensity = std::min(1.0f, std::max(0.0f, intensity) + 0.1f);
            return in.vertex.xyz1() * mat_mvp;
        }

        bool frag(sr_color &color) override {
            if (intensity > 0) {
                color.set(intensity, intensity, intensity, 1);
            }
            return true;
        }
    } phong_shader;
}

#endif //SHEEPRENDER_SR_PHONG_SHADER_H
