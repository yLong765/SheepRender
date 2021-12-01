//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_CAMERA_H
#define SHEEPRENDER_SR_CAMERA_H

namespace SR {
    // 相机类
    typedef struct sr_camera {
        vec3f from{};   // 相机位置
        vec3f to{};     // 相机观察位置
        vec3f up{};     // 相机向上的轴(此项目y轴向上)
        float n;        // 近平面
        float f;        // 远平面
        float fovy;     // 视场角
        vec3f zaxis;    // z轴
        vec3f xaxis;    // x轴
        vec3f yaxis;    // y轴

        sr_camera(vec3f from, vec3f to, vec3f up, float n = 1.0f, float f = 500.0f, float fovy = PI * 0.5f) {
            this->from = from;
            this->to = to;
            this->up = up;
            this->n = n;
            this->f = f;
            this->fovy = fovy;
        }

        // 获取相机朝向
        vec3f forward() {
            return zaxis;
        }

        // 获取相机观察矩阵
        mat4x4f get_look_at_matrix() {
            zaxis = vec_normalize(to - from);
            xaxis = vec_normalize(vec_cross(up, zaxis));
            yaxis = vec_cross(zaxis, xaxis);

            mat4x4f ret;
            ret.set_row(0, {xaxis.x, yaxis.x, zaxis.x, 0});
            ret.set_row(1, {xaxis.y, yaxis.y, zaxis.y, 0});
            ret.set_row(2, {xaxis.z, yaxis.z, zaxis.z, 0});
            ret.set_row(3, {-vec_dot(xaxis, from), -vec_dot(yaxis, from), -vec_dot(zaxis, from), 1});
            return ret;
        }

        // 获取相机正交矩阵（变换到齐次坐标）
        mat4x4f get_orthographic_matrix(int w, int h) const {
            mat4x4f ret;
            ret.set_row(0, vec4f(2 / w, 0, 0, 0));
            ret.set_row(1, vec4f(0, 2 / h, 0, 0));
            ret.set_row(2, vec4f(0, 0, 1 / (f - n), 0));
            ret.set_row(3, vec4f(0, 0, n / (f - n), 1));
            return ret;
        }

        // 获取相机透视矩阵（变换到齐次坐标）
        mat4x4f get_perspective_matrix(float aspect) const {
            float fax = 1.0f / (float) std::tan(fovy * 0.5f);
            mat4x4f ret = mat4x4f::zero();
            ret[0][0] = (float) (fax / aspect);
            ret[1][1] = (float) (fax);
            ret[2][2] = f / (f - n);
            ret[3][2] = -n * f / (f - n);
            ret[2][3] = 1;
            return ret;
        }

        // 齐次坐标归一化
        vec4f homogenize(const vec4f v, int w, int h) {
            vec4f ret;
            float rhw = 1.0f / v.w; // w的倒数
            ret.x = (v.x * rhw + 1.0f) * w * 0.5f;
            ret.y = (1.0f - v.y * rhw) * h * 0.5f;
            ret.z = v.z * rhw;
            ret.w = 1.0f;
            return ret;
        }
    } camera;
}

#endif //SHEEPRENDER_SR_CAMERA_H
