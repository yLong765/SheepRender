//
// Created by long on 2021/12/2.
//

#ifndef SHEEPRENDER_SR_RENDER_INC_H
#define SHEEPRENDER_SR_RENDER_INC_H

#include "sr_render.h"
#include "sr_object_render.h"

namespace SR {
    void sr_render::draw_obj(sr_object obj, RENDER_TYPE type) {
        switch (type) {
            case BARYCENTRIC_COORDINATE:
                auto render = new sr_object_render(obj);
                render->draw();
                break;
                case EDGE_EQUATION:
                    break;
                case EDGE_WALKING:
                    break;
                case DEPTH_RENDER:
                    //draw_depth(obj);
                    break;
        }
    }
}

#endif //SHEEPRENDER_SR_RENDER_INC_H
