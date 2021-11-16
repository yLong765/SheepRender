#include "main.h"

using namespace SR;

int main() {
    screen screen = screen::create(800, 600, "sheep render");

    MSG msg;
    while (GetMessage(&msg, screen.handle, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        screen.update();
    }

    return 0;
}