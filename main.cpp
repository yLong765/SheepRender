#include "main.h"

using namespace SR;

int main() {
    mat2x2 m = mat2x2::identity();
    m.set_translation({1, 2, 3});
    std::cout << m << std::endl;
    return 0;
}
