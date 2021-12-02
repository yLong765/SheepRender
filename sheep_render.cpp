#include "sheep_render.h"

using namespace SR;

int width = 800, height = 600;

int main() {
    screen screen = create_screen(width, height, "Sheep Render");
    camera camera(vec3f(0, 2, -4.0f), vec3f::zero(), vec3f::up());
    light light({1, 1, 1}, {1, 2, 1});
    object obj("../model/cube.obj");
    obj.set_shader(PHONG_SHADER);
    texture_2d texture(width, height);
    render render(&texture, &camera, &light);

    vec2f prev_mouse_point;
    vec2f mouse_change = vec2f(0, 0);
    bool first = true;

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

        sr_clock::begin();

        render.clear_color(color(0.0f, 0.0f, 0.0f));
        render.clear_z_buffer();
        render.draw_obj(obj);

        screen.set_buffer(&texture);
        screen.update_hdc();

        double detailTime = sr_clock::end();
        char *title_char = new char[40];
        sprintf(title_char, "Sheep Render  FPS: %d  %.0lf ms", (int) (1 / detailTime), detailTime * 1000);
        screen.set_title(title_char);
    }

    return 0;
}