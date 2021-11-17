//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_TEXCOORD_H
#define SHEEPRENDER_SR_TEXCOORD_H

namespace SR {
    typedef struct sr_texcoord {
        float u, v;

        sr_texcoord() = default;

        sr_texcoord(int u, int v) : u(u), v(v) {}
    } texcoord;
}

#endif //SHEEPRENDER_SR_TEXCOORD_H
