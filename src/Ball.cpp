#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            // Handle the WM_ERASEBKGND message to change the background color
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam; // Get the device context
            RECT rect;
            GetClientRect(hwnd, &rect); // Get the client area rectangle
            
            // Set the background color (e.g., light blue)
            HBRUSH hBrush = CreateSolidBrush(RGB(17, 11, 23));
            FillRect(hdc, &rect, hBrush); // Fill the background with the brush

            DeleteObject(hBrush); // Clean up the brush
            return 1; // Indicate that the background has been erased
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set the color for the ball
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Red color
            SelectObject(hdc, hBrush);

            // Draw the ball (an ellipse)
            Ellipse(hdc, 50, 50, 150, 150); // x1, y1, x2, y2

            // Clean up
            DeleteObject(hBrush);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BallWindowClass";

    // Register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Ball in a Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}