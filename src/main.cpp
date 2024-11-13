#include <windows.h>

// Define a Window Procedure function that handles messages sent to the window
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0); // Close the window
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Define and register a window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BasicWindowClass";

    RegisterClass(&wc);

    // Create a window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        "BasicWindowClass",             // Window class name
        "My Basic Window",              // Window title
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Window position (x, y)
        500, 300,                       // Window size (width, height)
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);

    // Main message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
