//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_MACRO_H
#define SHEEPRENDER_SR_MACRO_H

#define PI 3.1415926f
#define deg2rad(deg) ((PI / 180) * (deg))
#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))
#define aspect(w, h) ((float)w / (float)h)

#endif //SHEEPRENDER_SR_MACRO_H
