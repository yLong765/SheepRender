//
// Created by long on 2021/11/19.
//

#ifndef SHEEPRENDER_SR_OBJECT_MGR_H
#define SHEEPRENDER_SR_OBJECT_MGR_H

namespace SR {
    typedef struct sr_object_mgr {
        static std::vector<sr_object *> objects;

        static void load_model(const char *path) {
            objects.push_back(new sr_object(path));
        }

        static std::vector<sr_object *> get_objs() {
            return objects;
        }
    } object_mgr;

    std::vector<sr_object *> sr_object_mgr::objects;
}

#endif //SHEEPRENDER_SR_OBJECT_MGR_H
