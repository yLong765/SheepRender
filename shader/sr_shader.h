//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_SHADER_H
#define SHEEPRENDER_SR_SHADER_H

namespace SR {
    // shader类型
    enum SHADER_TYPE {
        CONSTANT_SHADER,
        PHONG_SHADER,
    };

    // shader插值所用key
    enum SHADER_KEY_TYPE {
        VERTEX_MODEL = 0,
        VERTEX_CLIP = 1,
        NORMAL_MODEL = 2,
        NORMAL_WORLD = 3,
        VIEW_WORLD = 4,
    };

    // shader顶点插值数据
    typedef struct vert_data {
        std::map<SHADER_KEY_TYPE, vec1f> v1f;
        std::map<SHADER_KEY_TYPE, vec2f> v2f;
        std::map<SHADER_KEY_TYPE, vec3f> v3f;
        std::map<SHADER_KEY_TYPE, vec4f> v4f;
    } vert_in, vert_out, frag_in;

    typedef struct sr_shader {
        mat4x4f mat_model;  // 模型转世界矩阵
        mat4x4f mat_view;   // 世界转相机矩阵
        mat4x4f mat_proj;   // 相机转齐次坐标矩阵
        vec3f view_pos;     // 相机位置

        // 灯光数据
        struct Light {
            vec3f color;        // 灯光颜色
            vec3f direction;    // 灯光方向
            vec3f position;     // 灯光位置
        } light;

        sr_shader() = default;

        // 获取MVP混合矩阵
        mat4x4f get_mvp() const { return mat_model * mat_view * mat_proj; }

        // 顶点处理
        virtual vert_out vert(vert_in in) = 0;

        // 像素处理
        virtual bool frag(frag_in in, sr_color &color) = 0;
    } shader;
}

#endif //SHEEPRENDER_SR_SHADER_H
