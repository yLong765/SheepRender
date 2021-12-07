//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_OBJECT_H
#define SHEEPRENDER_SR_OBJECT_H

namespace SR {
    // 场景物体基类
    typedef struct sr_object {
        std::map<COMPONENT_TYPE, sr_component *> comps;

        sr_object(const char *path) {
            add_comp(new sr_transform());
            add_comp(obj_loader::load(path));
        }

        void add_comp(sr_component *comp) {
            comps[comp->type] = comp;
        }

        void del_comp(COMPONENT_TYPE type) {
            comps.erase(type);
        }

        void del_comp(sr_component *comp) {
            comps.erase(comp->type);
        }

        template<typename T>
        T get_comp(COMPONENT_TYPE type) {
            if (comps.find(type) != comps.end()) {
                return dynamic_cast<T>(comps[type]);
            }
            return nullptr;
        }

        sr_transform *get_trans() {
            return get_comp<sr_transform *>(COMP_TRANSFORM);
        }

        sr_mesh *get_mesh() {
            return get_comp<sr_mesh *>(COMP_MESH);
        }

        // 设置shader
        void set_shader(SHADER_TYPE type) {
            get_mesh()->set_shader(type);
        }
    } object;
}

#endif //SHEEPRENDER_SR_OBJECT_H
