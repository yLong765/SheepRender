//
// Created by long on 2021/12/7.
//

#ifndef SHEEPRENDER_SR_COMPONENT_H
#define SHEEPRENDER_SR_COMPONENT_H

namespace SR {
    enum COMPONENT_TYPE {
        COMP_TRANSFORM = 0,
        COMP_MESH = 1,
        COMP_CAMERA = 2,
        COMP_LIGHT = 3,
    };

    typedef struct sr_component {
        COMPONENT_TYPE type;

        sr_component() = default;

        virtual ~sr_component() = default;
    } component;
}

#endif //SHEEPRENDER_SR_COMPONENT_H
