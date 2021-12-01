//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_MACRO_H
#define SHEEPRENDER_SR_MACRO_H

#define PI 3.1415926f // PI数据
#define deg2rad(deg) ((PI / 180) * (deg))   // 弧度转角度
#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))    // x控制在min和max之间
#define aspect(w, h) ((float)w / (float)h)  // 屏幕宽高比

#endif //SHEEPRENDER_SR_MACRO_H
