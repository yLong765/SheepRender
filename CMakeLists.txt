cmake_minimum_required(VERSION 3.20)
project(SheepRender)

set(CMAKE_CXX_STANDARD 14)

add_executable(SheepRender sheep_render.cpp math/sr_matrix.h math/sr_vector.h sheep_render.h base_data/sr_screen.h component/sr_transform.h object/sr_object.h base_data/sr_color.h component/sr_mesh.h render/sr_render.h component/sr_camera.h shader/sr_shader.h shader/sr_constant_shader.h asset_loader/sr_obj_loader.h base_data/sr_texture_2d.h object/sr_object_mgr.h math/sr_math.h component/sr_light.h shader/sr_phong_shader.h utility/sr_clock.h render/sr_barycentric_render.h base_data/sr_input.h render/sr_depth_render.h utility/sr_singleton.h render/sr_mesh_render.h render/sr_base_render.h render/sr_wireframe_render.h component/sr_component.h)
