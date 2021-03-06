//
// Created by long on 2021/11/16.
//

#ifndef SHEEPRENDER_SHEEP_RENDER_H
#define SHEEPRENDER_SHEEP_RENDER_H

// base
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <cfloat>
#include <windows.h>
#include <ctime>

// utility
#include "utility/sr_singleton.h"
#include "utility/sr_clock.h"

// math
#include "math/sr_vector.h"
#include "math/sr_matrix.h"
#include "math/sr_math.h"

// base data or component
#include "base_data/sr_color.h"
#include "base_data/sr_texture_2d.h"
#include "base_data/sr_screen.h"
#include "base_data/sr_input.h"

// shader
#include "shader/sr_shader.h"
#include "shader/sr_constant_shader.h"
#include "shader/sr_phong_shader.h"

// component
#include "component/sr_component.h"
#include "component/sr_transform.h"
#include "component/sr_mesh.h"
#include "component/sr_light.h"
#include "component/sr_camera.h"

// asset loader
#include "asset_loader/sr_obj_loader.h"

// object
#include "object/sr_object.h"
#include "object/sr_object_mgr.h"

// render
#include "render/sr_base_render.h"
#include "render/sr_wireframe_render.h"
#include "render/sr_mesh_render.h"
#include "render/sr_barycentric_render.h"
#include "render/sr_edge_equation_render.h"
#include "render/sr_depth_render.h"
#include "render/sr_render.h"

#endif //SHEEPRENDER_SHEEP_RENDER_H
