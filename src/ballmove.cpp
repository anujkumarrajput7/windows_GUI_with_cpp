#include <windows.h>
#include <cmath>
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void motion(HWND hwnd);

// Global variables for the ball's position
int ballX = 150; // Initial x position of the ball
int ballY = 150; // Initial y position of the ball
float speed = 5.0f; // Speed of the ball's movement

// This function handles the motion of the ball
void motion(HWND hwnd) {
    // Redraw the window if the ball's position has changed
    InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // Handle keyboard input for arrow keys
        case WM_KEYDOWN: {
            // Adjust the ball's position based on which arrow key is pressed
            switch (wParam) {
                case VK_LEFT:   // Left arrow key
                    ballX -= speed;
                    break;
                case VK_RIGHT:  // Right arrow key
                    ballX += speed;
                    break;
                case VK_UP:     // Up arrow key
                    ballY -= speed;
                    break;
                case VK_DOWN:   // Down arrow key
                    ballY += speed;
                    break;
            }
            motion(hwnd); // Update the ball's movement on the window
            return 0;
        }

        // Handle the WM_ERASEBKGND message to change the background color
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam; // Get the device context
            RECT rect;
            GetClientRect(hwnd, &rect); // Get the client area rectangle
            HBRUSH hBrush = CreateSolidBrush(RGB(173, 216, 230)); // Light blue color
            FillRect(hdc, &rect, hBrush); // Fill the background with the brush
            DeleteObject(hBrush);
            return 1;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Red color for the ball
            SelectObject(hdc, hBrush);
            Ellipse(hdc, ballX - 50, ballY - 50, ballX + 50, ballY + 50); // Draw the ball
            DeleteObject(hBrush);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BallWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Ball Movement with Arrow Keys", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg;
    DWORD lastTime = GetTickCount();

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Ensure the window refreshes at 60Hz (~16.67ms per frame)
        DWORD currentTime = GetTickCount();
        if (currentTime - lastTime >= 16) { // Check if 16ms have passed
            motion(hwnd); // Update the ball's position
            lastTime = currentTime;
        }

        // Sleep for a short time to control the animation speed
        Sleep(1); // Optional, but helps maintain control over CPU usage
    }

    return 0;
}
