//
// Created by long on 2021/12/3.
//

#ifndef SHEEPRENDER_SR_SINGLETON_H
#define SHEEPRENDER_SR_SINGLETON_H

namespace SR {
    template<typename T>
    struct sr_singleton {
    protected:
        sr_singleton() = default;

        ~sr_singleton() = default;

    public:
        static T &instance() {
            static T instance;
            return instance;
        }
    };
}

#endif //SHEEPRENDER_SR_SINGLETON_H
