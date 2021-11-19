#include "sheep_render.h"

using namespace SR;

int width = 800, height = 600;

int main() {
    screen screen = create_screen(width, height, "sheep render");
    camera camera(vec3(0, 0, -3.5f), vec3::zero(), vec3::up(), width, height);
    object model("../model/shape.obj");
    texture_2d texture(width, height);
    render render(&texture);

    while (screen_exit == 0) {
        screen.dispatch();

        if (keys[VK_UP]) model.transform.euler.x -= 1;
        if (keys[VK_DOWN]) model.transform.euler.x += 1;
        if (keys[VK_LEFT]) model.transform.euler.y -= 1;
        if (keys[VK_RIGHT]) model.transform.euler.y += 1;

        render.clear_color(color(0.0f, 0.0f, 0.0f));

        mat4x4 world = model.transform.get_world_matrix();
        mat4x4 view = camera.get_look_at_matrix();
        mat4x4 projection = camera.get_perspective_matrix();
        mat4x4 trans = world * view * projection;
        for (int i = 0; i < model.mesh.triangles.size(); i += 3) {
            vec4 screen_point[3];
            for (int j = 0; j < 3; j++) {
                int id = model.mesh.triangles[i + j];
                screen_point[j] = vec4(model.mesh.vertices[id], 1) * trans;
                screen_point[j] = camera.homogenize(screen_point[j]);
            }
            render.draw_triangle_wireframe(screen_point[0], screen_point[1], screen_point[2], color(1.0f, 1.0f, 1.0f));
        }

        screen.set_buffer(&texture);
        screen.update_hdc();
        Sleep(1);
    }

    return 0;
}