//
// Created by long on 2021/11/24.
//

#ifndef SHEEPRENDER_SR_LIGHT_H
#define SHEEPRENDER_SR_LIGHT_H

namespace SR {
    enum LIGHT_TYPE {
        LIGHT_DIRECTIONAL,
    };

    typedef struct sr_light {
        vec3f direction{};
        LIGHT_TYPE light_type;

        sr_light() {
            light_type = LIGHT_DIRECTIONAL;
            direction = vec3f(0, -1, 0);
        }

        vec3f get_light_dir() const {
            switch (light_type) {
                case LIGHT_DIRECTIONAL:
                    return -direction;
            }
        }
    } light;
}

#endif //SHEEPRENDER_SR_LIGHT_H