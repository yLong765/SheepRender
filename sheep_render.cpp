#include "sheep_render.h"

using namespace SR;

int width = 800, height = 600;

int main() {
    screen screen = create_screen(width, height, "sheep render");
    camera camera(vec3(0, 0, -3.5f), vec3::zero(), vec3::up(), width, height);
    object obj("../model/cube2.obj");
    texture_2d texture(width, height);
    render render(&texture, &camera);

    vec2 prev_mouse_point;
    vec2 mouse_change = vec2(0, 0);
    bool first = true;

    while (screen_exit == 0) {
        screen.dispatch();

        if (keys[VK_LBUTTON]) {
            if (first) {
                prev_mouse_point = screen.get_mouse_point();
            } else {
                vec2 new_mouse_point = screen.get_mouse_point();
                mouse_change = new_mouse_point - prev_mouse_point;
                prev_mouse_point = new_mouse_point;
            }
            first = false;
        } else {
            first = true;
            mouse_change = vec2(0, 0);
        }

        render.clear_color(color(0.0f, 0.0f, 0.0f));

        obj.transform.euler.x -= mouse_change.y * 0.1f;
        obj.transform.euler.y += mouse_change.x * 0.1f;

        render.draw_wireframe(obj);

        screen.set_buffer(&texture);
        screen.update_hdc();
        Sleep(1);
    }

    return 0;
}