//
// Created by long on 2021/11/16.
//

#ifndef SHEEPRENDER_SR_SCREEN_H
#define SHEEPRENDER_SR_SCREEN_H

#include <windows.h>

namespace SR {
    static int keys[512];

    struct sr_screen {
        HWND handle;
        int width;
        int height;
        HDC memory_dc;

        void update() const {
            HDC hDC = GetDC(handle);
            BitBlt(hDC, 0, 0, width, height, memory_dc, 0, 0, SRCCOPY);
            ReleaseDC(handle, hDC);
        }

    private:
        static LRESULT CALLBACK event_screen(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            switch (msg) {
                case WM_CLOSE:
                    DestroyWindow(hWnd);
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
                default:
                    return DefWindowProc(hWnd, msg, wParam, lParam);
            }
            return 0;
        }

        static WNDCLASS register_class(const char *title) {
            WNDCLASS wc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpfnWndProc = event_screen;
            wc.lpszClassName = title;
            wc.lpszMenuName = nullptr;
            wc.style = CS_BYTEALIGNCLIENT;
            return wc;
        }

    public:
        static sr_screen create(int width, int height, const char *title) {
            WNDCLASS wc = register_class(title);
            assert(RegisterClass(&wc));

            HWND handle = CreateWindow(title, title,
                                       WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                       0, 0, 0, 0,
                                       nullptr, nullptr,
                                       wc.hInstance, nullptr);
            assert(handle != nullptr);

            HDC window_dc = GetDC(handle);
            HDC memory_dc = CreateCompatibleDC(window_dc);
            ReleaseDC(handle, window_dc);

            // 分配bitmap和framebuffer
            LPVOID ptr;
            BITMAPINFOHEADER bitmap_header;
            memset(&bitmap_header, 0, sizeof(BITMAPINFOHEADER));
            bitmap_header.biSize = sizeof(BITMAPINFOHEADER);
            bitmap_header.biWidth = width;
            bitmap_header.biHeight = height;
            bitmap_header.biPlanes = 1;
            bitmap_header.biBitCount = 32;
            bitmap_header.biCompression = BI_RGB;
            HBITMAP hbitmap = CreateDIBSection(memory_dc, (BITMAPINFO *) &bitmap_header,
                                               DIB_RGB_COLORS, &ptr, nullptr, 0);
            assert(hbitmap != nullptr);
            HBITMAP old_bitmap = (HBITMAP) SelectObject(memory_dc, hbitmap);
            DeleteObject(old_bitmap);
            // 分配bitmap和framebuffer

            // 调整界面大小
            RECT rect = {0, 0, width, height};
            AdjustWindowRect(&rect, GetWindowLong(handle, GWL_STYLE), 0);
            int wx = rect.right - rect.left;
            int wy = rect.bottom - rect.top;
            int sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
            int sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
            if (sy < 0) sy = 0;
            SetWindowPos(handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
            // 调整界面大小

            memset(keys, 0, (sizeof(int) * 512));

            ShowWindow(handle, SW_SHOW);
            UpdateWindow(handle);

            sr_screen screen{};
            screen.handle = handle;
            screen.width = width;
            screen.height = height;
            screen.memory_dc = memory_dc;
            return screen;
        }
    };

    typedef sr_screen screen;
}

#endif //SHEEPRENDER_SR_SCREEN_H
