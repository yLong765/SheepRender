cmake_minimum_required(VERSION 3.20)
project(SheepRender)

set(CMAKE_CXX_STANDARD 14)

add_executable(SheepRender sheep_render.cpp sr_matrix.h sr_vector.h sheep_render.h sr_screen.h sr_transform.h sr_object.h sr_color.h sr_texcoord.h sr_mesh.h sr_macro.h sr_render.h sr_camera.h shader/sr_shader.h shader/sr_shader_inc.h shader/sr_phong_shader.h sr_obj_loader.h)
