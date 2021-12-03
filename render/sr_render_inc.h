//
// Created by long on 2021/12/2.
//

#ifndef SHEEPRENDER_SR_RENDER_INC_H
#define SHEEPRENDER_SR_RENDER_INC_H

#include "sr_render.h"
#include "sr_object_render.h"

namespace SR {
    sr_render *create_render(sr_texture_2d *texture, sr_camera *camera, sr_light *light,
                             RENDER_TYPE type = BARYCENTRIC_COORDINATE) {
        switch (type) {
            case BARYCENTRIC_COORDINATE:
                return new sr_object_render(texture, camera, light);
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
