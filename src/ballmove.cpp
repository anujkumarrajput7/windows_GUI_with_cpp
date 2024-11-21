#include <windows.h>
#include <algorithm> // To use std::max and std::min

// Constants
const int BALL_SIZE = 50; // Diameter of the ball
const int FPS = 60; // Frames per second
const float MAX_SPEED = 1000.0f / FPS; // Maximum speed (1000 px/sec)
const float MIN_SPEED = 100.0f / FPS;  // Minimum speed (100 px/sec)
const float SPEED_STEP = 100.0f / FPS; // Speed increment/decrement
const COLORREF BACKGROUND_COLOR = RGB(173, 216, 230); // Light blue
const COLORREF BALL_COLOR = RGB(255, 0, 0);           // Red

// Ball position and speed
int ballX = 150, ballY = 150;
float ballSpeed = 500.0f / FPS; // Initial speed

// Keys pressed state
bool keyUp = false, keyDown = false, keyLeft = false, keyRight = false;

// Function to handle ball movement
void moveBall(RECT& clientRect) {
    if (keyUp) 
        ballY = std::max(0, ballY - static_cast<int>(ballSpeed));
    if (keyDown) 
        ballY = std::min(static_cast<int>(clientRect.bottom) - BALL_SIZE, ballY + static_cast<int>(ballSpeed));
    if (keyLeft) 
        ballX = std::max(0, ballX - static_cast<int>(ballSpeed));
    if (keyRight) 
        ballX = std::min(static_cast<int>(clientRect.right) - BALL_SIZE, ballX + static_cast<int>(ballSpeed));
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            // Handle arrow keys for movement
            if (wParam == VK_UP) keyUp = true;
            if (wParam == VK_DOWN) keyDown = true;
            if (wParam == VK_LEFT) keyLeft = true;
            if (wParam == VK_RIGHT) keyRight = true;

            // Adjust speed with W (increase) and S (decrease)
            if (wParam == 'W') ballSpeed = std::min(MAX_SPEED, ballSpeed + SPEED_STEP);
            if (wParam == 'S') ballSpeed = std::max(MIN_SPEED, ballSpeed - SPEED_STEP);
            return 0;

        case WM_KEYUP:
            // Reset directional keys
            if (wParam == VK_UP) keyUp = false;
            if (wParam == VK_DOWN) keyDown = false;
            if (wParam == VK_LEFT) keyLeft = false;
            if (wParam == VK_RIGHT) keyRight = false;
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Clear the background
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrushBackground = CreateSolidBrush(BACKGROUND_COLOR);
            FillRect(hdc, &rect, hBrushBackground);
            DeleteObject(hBrushBackground);

            // Draw the ball
            HBRUSH hBrushBall = CreateSolidBrush(BALL_COLOR);
            SelectObject(hdc, hBrushBall);
            Ellipse(hdc, ballX, ballY, ballX + BALL_SIZE, ballY + BALL_SIZE);
            DeleteObject(hBrushBall);

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
        0, CLASS_NAME, "Ball Speed Control", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    // Inside the main game loop
while (true) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    } else {
        // Get the window client area
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // Move the ball and keep it within bounds
        moveBall(clientRect);

        // Redraw the window
        InvalidateRect(hwnd, NULL, TRUE);

         // Control refresh rate (240 FPS)
        Sleep(4); // Sleep for 4 milliseconds (approx. 240 FPS)
    }
}


    return 0;
}
