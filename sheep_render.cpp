#include "sheep_render.h"

using namespace SR;

int width = 800, height = 600;

int main() {
    screen *screen = create_screen(width, height, "Sheep Render");
    input input(screen);
    camera camera(vec3f(0, 2, -4.0f), vec3f::zero(), vec3f::up());
    light light({1, 1, 1}, {1, 2, 1});
    texture_2d texture(width, height);
    object_mgr::load_model("../model/cube.obj");
    std::vector<sr_object *> objs = object_mgr::get_objs();
    object *obj = objs[0];
    render::instance().set_texture(&texture);
    render::instance().set_camera(&camera);
    render::instance().set_light(&light);

    while (screen_exit == 0) {
        input.update();
        if (keys[VK_MOUSEWHEELUP]) camera.from.z += 0.1f;
        if (keys[VK_MOUSEWHEELDOWN]) camera.from.z -= 0.1f;
        vec2f change = input.get_mouse_change();
        obj->transform.euler.x += change.y * 0.2f;
        obj->transform.euler.y += change.x * 0.2f;

        sr_clock::instance().begin();

        render::instance().clear_color(color(0.0f, 0.0f, 0.0f));
        render::instance().clear_z_buffer();
        render::instance().draw_mesh(DEPTH_RENDER);

        screen->set_buffer(&texture);
        screen->update_hdc();

        double detailTime = sr_clock::instance().end();
        char *title_char = new char[40];
        sprintf(title_char, "Sheep Render  FPS: %d  %.0lf ms", (int) (1 / detailTime), detailTime * 1000);
        screen->set_title(title_char);
    }

    return 0;
}