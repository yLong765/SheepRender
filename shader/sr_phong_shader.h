//
// Created by long on 2021/11/24.
//

#ifndef SHEEPRENDER_SR_PHONG_SHADER_H
#define SHEEPRENDER_SR_PHONG_SHADER_H

namespace SR {
    typedef struct sr_phong_shader : public sr_shader {
        sr_phong_shader() = default;

        vert_out vert(vert_in in) override {
            vert_out out;
            out.v4f[VERTEX_CLIP] = in.v3f[VERTEX_MODEL].xyz1() * get_mvp();
            out.v3f[NORMAL_WORLD] = vec_normalize(in.v3f[NORMAL_MODEL] * mat_invert(mat_model).transpose());
            vec4f vert_pos = in.v3f[VERTEX_MODEL].xyz1() * mat_model;
            out.v3f[VIEW_WORLD] = vec_normalize(view_pos - vert_pos.xyz());
            return out;
        }

        bool frag(frag_in in, sr_color &color) override {
            float ambi = 0.1f, diff = 0.4f, spec = 0.5f;
            vec3f ambient = ambi * light.color;
            vec3f view_dir = in.v3f[VIEW_WORLD];
            vec3f normal_dir = in.v3f[NORMAL_WORLD];
            vec3f light_dir = vec_normalize(-light.direction);
            vec3f diffuse = diff * std::max(vec_dot(normal_dir, light_dir), 0.0f) * light.color;
            vec3f rel = vec_normalize(normal_dir * vec_dot(normal_dir, light_dir) * 2.0f - light_dir);
            vec3f specular = spec * std::pow(std::max(vec_dot(view_dir, rel), 0.0f), 32) * light.color;
            color.set(ambient + diffuse + specular);
            return true;
        }
    } phong_shader;
}

#endif //SHEEPRENDER_SR_PHONG_SHADER_H