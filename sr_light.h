//
// Created by long on 2021/11/24.
//

#ifndef SHEEPRENDER_SR_LIGHT_H
#define SHEEPRENDER_SR_LIGHT_H

namespace SR {
    // 灯光类型
    enum LIGHT_TYPE {
        // 方向光
        LIGHT_DIRECTIONAL,
        // 点光源
        LIGHT_POINT,
    };

    typedef struct sr_light {
        LIGHT_TYPE light_type;  // 光类型
        sr_color color;         // 光颜色
        vec3f position{};       // 光位置
        vec3f direction{};      // 方向光数据
        float range{};          // 点光源数据

        sr_light(sr_color color, vec3f position) {
            this->position = position;
            this->color = color;
            set_direction_light({0, -1, 0});
        }

        // 设置方向光数据
        void set_direction_light(vec3f direction) {
            this->light_type = LIGHT_DIRECTIONAL;
            this->direction = direction;
        }

        // 设置点光源数据
        void set_point_light(float range) {
            this->range = range;
        }
    } light;
}

#endif //SHEEPRENDER_SR_LIGHT_H