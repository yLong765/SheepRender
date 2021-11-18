//
// Created by long on 2021/11/17.
//

#ifndef SHEEPRENDER_SR_OBJECT_H
#define SHEEPRENDER_SR_OBJECT_H

namespace SR {
    typedef struct sr_object {
        sr_transform transform;
        sr_mesh mesh;

        sr_object(const char *path) {
            mesh = obj_loader::load(path);
        }
    } object;
}

#endif //SHEEPRENDER_SR_OBJECT_H
