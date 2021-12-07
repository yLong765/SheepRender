//
// Created by long on 2021/12/3.
//

#ifndef SHEEPRENDER_SR_INPUT_H
#define SHEEPRENDER_SR_INPUT_H

namespace SR {
    typedef struct sr_input : public sr_singleton<sr_input> {
    private:
        vec2f prev_mouse_point;
        vec2f mouse_change;
        bool first;
    public:
        sr_input() {
            mouse_change = vec2f(0, 0);
            first = true;
        }

        void update() {
            keys[VK_MOUSEWHEELUP] = 0;
            keys[VK_MOUSEWHEELDOWN] = 0;

            screen->dispatch();

            if (keys[VK_LBUTTON]) {
                if (first) {
                    prev_mouse_point = screen::instance().get_mouse_point();
                } else {
                    vec2f new_mouse_point = screen::instance().get_mouse_point();
                    mouse_change = new_mouse_point - prev_mouse_point;
                    prev_mouse_point = new_mouse_point;
                }
                first = false;
            } else {
                first = true;
                mouse_change = vec2f(0, 0);
            }
        }

        vec2f get_mouse_change() {
            return mouse_change;
        }
    } input;
}

#endif //SHEEPRENDER_SR_INPUT_H
