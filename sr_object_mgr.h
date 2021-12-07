//
// Created by long on 2021/11/19.
//

#ifndef SHEEPRENDER_SR_OBJECT_MGR_H
#define SHEEPRENDER_SR_OBJECT_MGR_H

namespace SR {
    typedef struct sr_object_mgr : public sr_singleton<sr_object_mgr> {
        std::vector<sr_object *> objects;

        void load_model(const char *path) {
            objects.push_back(new sr_object(path));
        }

        std::vector<sr_object *> get_objs() {
            return objects;
        }

        sr_object *get_first_obj() {
            if (!objects.empty()) {
                return objects[0];
            }
            return nullptr;
        }
    } object_mgr;
}

#endif //SHEEPRENDER_SR_OBJECT_MGR_H
