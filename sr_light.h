//
// Created by long on 2021/11/24.
//

#ifndef SHEEPRENDER_SR_LIGHT_H
#define SHEEPRENDER_SR_LIGHT_H

namespace SR {
    enum LIGHT_TYPE {
        LIGHT_DIRECTIONAL,
        LIGHT_POINT,
    };

    typedef struct sr_light {
        LIGHT_TYPE light_type;  // 光类型
        sr_color color;         // 光颜色
        vec3f position{};         // 光位置
        // 方向光数据
        vec3f direction{};
        // 点光源数据
        float range{};

        sr_light(sr_color color, vec3f position) {
            this->position = position;
            this->color = color;
            set_direction_light({0, -1, 0});
        }

        void set_direction_light(vec3f direction) {
            this->light_type = LIGHT_DIRECTIONAL;
            this->direction = direction;
        }

        void set_point_light(float range) {
            this->range = range;
        }
    } light;
}

#endif //SHEEPRENDER_SR_LIGHT_H