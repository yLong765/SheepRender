//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_CAMERA_H
#define SHEEPRENDER_SR_CAMERA_H

namespace SR {
    typedef struct sr_camera {
        vec3 from{};
        vec3 to{};
        vec3 up{};
        float n;
        float f;
        float fovy;

        sr_camera() {
            from = vec3(0, 0, -10);
            to = vec3::zero();
            up = vec3::up();
            n = 1.0f;
            f = 500.0f;
            fovy = PI * 0.5f;
        }

        sr_camera(vec3 from, vec3 to, vec3 up) {
            this->from = from;
            this->to = to;
            this->up = up;
            n = 1.0f;
            f = 500.0f;
            fovy = PI * 0.5f;
        }

        mat4x4 get_look_at_matrix() const {
            vec3 zaxis = (to - from).normalize();
            vec3 xaxis = vec3::cross(up, zaxis).normalize();
            vec3 yaxis = vec3::cross(zaxis, xaxis);

            mat4x4 ret;
            ret.set_row(0, {xaxis.x, yaxis.x, zaxis.x, 0});
            ret.set_row(1, {xaxis.y, yaxis.y, zaxis.y, 0});
            ret.set_row(2, {xaxis.z, yaxis.z, zaxis.z, 0});
            ret.set_row(3, {-(xaxis * from), -(yaxis * from), -(zaxis * from), 1});
            return ret;
        }

        mat4x4 get_orthographic_matrix(float w, float h) const {
            mat4x4 ret;
            ret.set_row(0, vec4(2 / w, 0, 0, 0));
            ret.set_row(1, vec4(0, 2 / h, 0, 0));
            ret.set_row(2, vec4(0, 0, 1 / (f - n), 0));
            ret.set_row(3, vec4(0, 0, n / (f - n), 1));
            return ret;
        }

        mat4x4 get_perspective_matrix(float w, float h) const {
            float fax = 1.0f / (float) std::tan(fovy * 0.5f);
            float aspect = w / h;
            mat4x4 ret = mat4x4::zero();
            ret[0][0] = (float) (fax / aspect);
            ret[1][1] = (float) (fax);
            ret[2][2] = f / (f - n);
            ret[3][2] = -n * f / (f - n);
            ret[2][3] = 1;
            return ret;
        }

        vec4 homogenize(float w, float h, const vec4 v) {
            vec4 ret;
            float rhw = 1.0f / v.w;
            ret.x = (v.x * rhw + 1.0f) * w * 0.5f;
            ret.y = (1.0f - v.y * rhw) * h * 0.5f;
            ret.z = v.z * rhw;
            ret.w = 1.0f;
            return ret;
        }
    } camera;
}

#endif //SHEEPRENDER_SR_CAMERA_H
