#include <iostream>
#include "sr_math.h"

using namespace SR;

int main() {
    vec3 v1(1, 2, 3);
    vec3 v = vec3::cross(v1, v1);

    mat2x2 m1 = mat2x2::identity();
    mat2x2 m2 = mat2x2::identity();
    m2.set_row(0, vec2(1, 2));
    m2.set_row(1, vec2(3, 4));

    auto m = m1 * m2;

    std::cout << m << std::endl;
    std::cout << m.transpose() << std::endl;
    return 0;
}
