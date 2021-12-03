//
// Created by long on 2021/11/26.
//

#ifndef SHEEPRENDER_SR_CLOCK_H
#define SHEEPRENDER_SR_CLOCK_H

namespace SR {
    // 简易计时器
    struct sr_clock : public sr_singleton<sr_clock> {
        clock_t prev_clock;

        // 开启计时
        void begin() {
            prev_clock = clock();
        }

        // 结束计时
        double end() {
            return (double) (clock() - prev_clock) / CLOCKS_PER_SEC;
        }
    };
}

#endif //SHEEPRENDER_SR_CLOCK_H
