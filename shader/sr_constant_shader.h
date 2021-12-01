//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_CONSTANT_SHADER_H
#define SHEEPRENDER_SR_CONSTANT_SHADER_H

namespace SR {
    // 常数光照
    typedef struct sr_constant_shader : public sr_shader {
        float intensity;

        sr_constant_shader() = default;

        vert_out vert(vert_in in) override {
            vert_out out;
            out.v4f[VERTEX_CLIP] = in.v3f[VERTEX_MODEL].xyz1() * get_mvp();
            // 世界坐标系的向量
            vec3f normal_w = vec_normalize(in.v3f[NORMAL_MODEL] * mat_invert(mat_model).transpose());
            // 与灯光（反）方向点乘，大于零则有光照到面片上
            intensity = vec_dot(normal_w, -light.direction);
            intensity = std::min(1.0f, std::max(0.0f, intensity) + 0.1f);
            return out;
        }

        bool frag(frag_in in, sr_color &color) override {
            if (intensity > 0) {
                color.set(intensity, intensity, intensity, 1);
            }
            return true;
        }
    } constant_shader;
}

#endif //SHEEPRENDER_SR_CONSTANT_SHADER_H
