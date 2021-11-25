#include "sheep_render.h"

using namespace SR;

int width = 800, height = 600;

int main() {
    screen screen = create_screen(width, height, "sheep render");
    camera camera(vec3f(0, 2, -3.5f), vec3f::zero(), vec3f::up());
    light light({1, 1, 1}, {1, 2, 1});
    object obj("../model/plane.obj");
    obj.set_shader(PHONG_SHADER);
    texture_2d texture(width, height);
    render render(&texture, &camera, &light);

    vec2f prev_mouse_point;
    vec2f mouse_change = vec2f(0, 0);
    bool first = true;
    float fps = 0.0f;
    clock_t cur_clock = clock();
    clock_t pre_clock = clock();

    while (screen_exit == 0) {
        keys[VK_MOUSEWHEELUP & 511] = 0;
        keys[VK_MOUSEWHEELDOWN & 511] = 0;

        screen.dispatch();

        if (keys[VK_MOUSEWHEELUP]) camera.from.z += 0.1f;
        if (keys[VK_MOUSEWHEELDOWN]) camera.from.z -= 0.1f;

        if (keys[VK_LBUTTON]) {
            if (first) {
                prev_mouse_point = screen.get_mouse_point();
            } else {
                vec2f new_mouse_point = screen.get_mouse_point();
                mouse_change = new_mouse_point - prev_mouse_point;
                prev_mouse_point = new_mouse_point;
            }
            first = false;
        } else {
            first = true;
            mouse_change = vec2f(0, 0);
        }

        obj.transform.euler.x += mouse_change.y * 0.2f;
        obj.transform.euler.y += mouse_change.x * 0.2f;

        render.clear_color(color(0.0f, 0.0f, 0.0f));
        render.clear_z_buffer();

        //render.draw_axis(obj);
        render.draw(obj);
        //render.draw_wireframe(obj, color(0.5f, 0.5f, 0.5f));
        //render.draw_normal(obj, color(0.0f, 1.0f, 0.0f));
        //render.draw_js_normal(obj, color(0.0f, 0.0f, 1.0f));

        screen.set_buffer(&texture);
        screen.update_hdc();

        // 计算fps
        cur_clock = clock();
        double detailTime = (double) (cur_clock - pre_clock) / CLOCKS_PER_SEC;
        pre_clock = cur_clock;

        std::string title = "FPS: " + std::to_string((int) (1000 / (detailTime * 1000)));

        screen.set_title(title.c_str());
        Sleep(1);
    }

    return 0;
}