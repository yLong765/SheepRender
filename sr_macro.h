//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_MACRO_H
#define SHEEPRENDER_SR_MACRO_H

#define PI 3.1415926f
#define aspect(width, height) ((float)width / (float)height)
#define deg2rad(deg) ((PI / 180) * (deg))

unsigned char CMID(unsigned char x, unsigned char min, unsigned char max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

#endif //SHEEPRENDER_SR_MACRO_H
