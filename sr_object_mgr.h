//
// Created by long on 2021/11/19.
//

#ifndef SHEEPRENDER_SR_OBJECT_MGR_H
#define SHEEPRENDER_SR_OBJECT_MGR_H

namespace SR {
    typedef struct sr_object_mgr {
        std::vector<sr_object> objects;

        void load_model(const char *path) {
            objects.emplace_back(path);
        }
    } object_mgr;
}

#endif //SHEEPRENDER_SR_OBJECT_MGR_H
