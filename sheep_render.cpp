#include "sheep_render.h"

using namespace SR;

int width = 800, height = 600;

int main() {
    screen::instance().init(width, height, "sheep render");
    camera camera(vec3f(0, 2, -4.0f), vec3f::zero(), vec3f::up());
    light light({1, 1, 1}, {1, 2, 1});
    texture_2d texture(width, height);
    object_mgr::instance().load_model("../model/cube.obj");
    render::instance().init(&texture, &camera, &light);
    sr_object *obj = object_mgr::instance().get_first_obj();

    while (screen_exit == 0) {
        input::instance().update();
        if (keys[VK_MOUSEWHEELUP]) camera.from.z += 0.1f;
        if (keys[VK_MOUSEWHEELDOWN]) camera.from.z -= 0.1f;
        vec2f change = input::instance().get_mouse_change();
        obj->get_trans()->euler.x += change.y * 0.2f;
        obj->get_trans()->euler.y += change.x * 0.2f;

        sr_clock::instance().begin();

        render::instance().clear();
        render::instance().draw_mesh(DEPTH_RENDER);
        render::instance().draw_axis(obj);

        screen::instance().set_buffer(&texture);
        screen::instance().update_hdc();

        double detailTime = sr_clock::instance().end();
        char *title_char = new char[40];
        sprintf(title_char, "Sheep Render  FPS: %d  %.0lf ms", (int) (1 / detailTime), detailTime * 1000);
        screen::instance().set_title(title_char);
    }

    return 0;
}