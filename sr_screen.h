//
// Created by long on 2021/11/16.
//

#ifndef SHEEPRENDER_SR_SCREEN_H
#define SHEEPRENDER_SR_SCREEN_H

namespace SR {
#define VK_MOUSEWHEELUP 500
#define VK_MOUSEWHEELDOWN 501

    static int keys[512];
    static int screen_exit;

    typedef struct sr_screen {
        HWND handle;
        int width;
        int height;
        HDC memory_dc;
        UINT *frame_buffer;

        void set_buffer(sr_texture_2d *texture) {
            int pixel_count = width * height;
            for (int i = 0; i < pixel_count; i++) {
                frame_buffer[i] = (*texture)[i].get_pixel_color();
            }
        }

        void update_hdc() {
            HDC hDC = GetDC(handle);
            BitBlt(hDC, 0, 0, width, height, memory_dc, 0, 0, SRCCOPY);
            ReleaseDC(handle, hDC);
        }

        void dispatch() const {
            MSG msg;
            GetMessage(&msg, nullptr, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        vec2f get_mouse_point() const {
            POINT point;
            GetCursorPos(&point);
            ScreenToClient(handle, &point);
            return vec2f{(float) point.x, (float) point.y};
        }
    } screen;

    static LRESULT CALLBACK event_screen(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_CLOSE:
                screen_exit = 1;
                CloseWindow(hWnd);
                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            case WM_KEYDOWN:
                keys[wParam & 511] = 1;
                break;
            case WM_KEYUP:
                keys[wParam & 511] = 0;
                break;
            case WM_LBUTTONDOWN:
                keys[VK_LBUTTON & 511] = 1;
                break;
            case WM_RBUTTONDOWN:
                keys[VK_RBUTTON & 511] = 1;
                break;
            case WM_LBUTTONUP:
                keys[VK_LBUTTON & 511] = 0;
                break;
            case WM_RBUTTONUP:
                keys[VK_RBUTTON & 511] = 0;
                break;
            case WM_MOUSEWHEEL:
                keys[VK_MOUSEWHEELUP & 511] = HIWORD(wParam) == WHEEL_DELTA;
                keys[VK_MOUSEWHEELDOWN & 511] = HIWORD(wParam) != WHEEL_DELTA;
            default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        return 0;
    }

    sr_screen create_screen(int width, int height, const char *title) {
        WNDCLASS wc;
        wc.style = CS_BYTEALIGNCLIENT;
        wc.lpfnWndProc = event_screen;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hIcon = nullptr;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);;
        wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = title;

        assert(RegisterClass(&wc));

        BITMAPINFO bi;
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = width;
        bi.bmiHeader.biHeight = height;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        bi.bmiHeader.biSizeImage = static_cast<DWORD>(width * height * 4);
        bi.bmiHeader.biXPelsPerMeter = 0;
        bi.bmiHeader.biYPelsPerMeter = 0;
        bi.bmiHeader.biClrUsed = 0;
        bi.bmiHeader.biClrImportant = 0;

        HWND handle = CreateWindow(title, title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                   0, 0, 0, 0, nullptr, nullptr, wc.hInstance, nullptr);
        assert(handle != nullptr);

        HDC screen_dc = GetDC(handle);
        HDC memory_dc = CreateCompatibleDC(screen_dc);
        ReleaseDC(handle, screen_dc);

        // 分配bitmap和framebuffer
        LPVOID ptr;
        HBITMAP hbitmap = CreateDIBSection(memory_dc, &bi, DIB_RGB_COLORS, &ptr, nullptr, 0);
        assert(hbitmap != nullptr);

        HBITMAP screen_ob = (HBITMAP) SelectObject(memory_dc, hbitmap);
        // 分配bitmap和framebuffer

        // 调整界面大小
        RECT rect = {0, 0, width, height};
        AdjustWindowRect(&rect, GetWindowLong(handle, GWL_STYLE), 0);
        int wx = rect.right - rect.left;
        int wy = rect.bottom - rect.top;
        int sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
        int sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
        if (sy < 0) sy = 0;
        SetWindowPos(handle, nullptr, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
        SetForegroundWindow(handle);

        ShowWindow(handle, SW_SHOW);

        memset(keys, 0, sizeof(int) * 512);

        sr_screen screen{};
        screen.handle = handle;
        screen.width = width;
        screen.height = height;
        screen.memory_dc = memory_dc;
        screen.frame_buffer = (UINT *) ptr;

        memset(screen.frame_buffer, 0, width * height * sizeof(UINT));

        return screen;
    }
}

#endif //SHEEPRENDER_SR_SCREEN_H
