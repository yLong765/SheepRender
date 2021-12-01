//
// Created by long on 2021/11/26.
//

#ifndef SHEEPRENDER_SR_CLOCK_H
#define SHEEPRENDER_SR_CLOCK_H

namespace SR {
    // 简易计时器
    struct sr_clock {
        static clock_t prev_clock;

        // 开启计时
        static void begin() {
            prev_clock = clock();
        }

        // 结束计时
        static double end() {
            return (double) (clock() - prev_clock) / CLOCKS_PER_SEC;
        }
    };

    clock_t sr_clock::prev_clock;
}

#endif //SHEEPRENDER_SR_CLOCK_H
