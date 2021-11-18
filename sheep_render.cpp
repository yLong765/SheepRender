#include "sheep_render.h"

using namespace SR;

int width = 800, height = 600;

int main() {
    screen screen = create_screen(width, height, "sheep render");
    camera camera(vec3(0, 0, -3.5f), vec3::zero(), vec3::up(), width, height);
    object model("../model/cube.obj");
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
            int id1 = model.mesh.triangles[i];
            int id2 = model.mesh.triangles[i + 1];
            int id3 = model.mesh.triangles[i + 2];
            vec4 p1 = vec4(model.mesh.vertices[id1], 1);
            vec4 p2 = vec4(model.mesh.vertices[id2], 1);
            vec4 p3 = vec4(model.mesh.vertices[id3], 1);
            p1 = p1 * trans;
            p2 = p2 * trans;
            p3 = p3 * trans;
            p1 = camera.homogenize(p1);
            p2 = camera.homogenize(p2);
            p3 = camera.homogenize(p3);
            render.draw_triangle_wireframe(p1, p2, p3, color(1.0f, 0.0f, 0.0f));
        }

        screen.set_buffer(&texture);
        screen.update_hdc();
        Sleep(1);
    }

    return 0;
}