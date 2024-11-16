#include <windows.h>
#include <algorithm> // To use std::max

// Ball position and speed
int ballX = 150, ballY = 150;
int ballSpeed = 5; // Initial speed of the ball

// Keys pressed state
bool keyUp = false, keyDown = false, keyLeft = false, keyRight = false;
bool keyLeftPressed = false, keyRightPressed = false; // Track taps for speed adjustment

// Function to handle ball movement
void moveBall() {
    if (keyUp) ballY -= ballSpeed; // Move up
    if (keyDown) ballY += ballSpeed; // Move down
    if (keyLeft) ballX -= ballSpeed; // Move left
    if (keyRight) ballX += ballSpeed; // Move right
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
            if (wParam == VK_LEFT) {
                keyLeft = true;
                if (!keyLeftPressed) { // Only adjust speed on new press
                    ballSpeed = std::max(1, ballSpeed - 1); // Decrease speed but keep it >= 1
                    keyLeftPressed = true; // Mark as handled
                }
            }
            if (wParam == VK_RIGHT) {
                keyRight = true;
                if (!keyRightPressed) { // Only adjust speed on new press
                    ballSpeed += 1; // Increase speed
                    keyRightPressed = true; // Mark as handled
                }
            }
            return 0;

        case WM_KEYUP:
            // Reset directional keys
            if (wParam == VK_UP) keyUp = false;
            if (wParam == VK_DOWN) keyDown = false;
            if (wParam == VK_LEFT) {
                keyLeft = false;
                keyLeftPressed = false; // Allow speed adjustment on next press
            }
            if (wParam == VK_RIGHT) {
                keyRight = false;
                keyRightPressed = false; // Allow speed adjustment on next press
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Clear the background
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrushBackground = CreateSolidBrush(RGB(173, 216, 230)); // Light blue background
            FillRect(hdc, &rect, hBrushBackground);
            DeleteObject(hBrushBackground);

            // Draw the ball
            HBRUSH hBrushBall = CreateSolidBrush(RGB(255, 0, 0)); // Red ball
            SelectObject(hdc, hBrushBall);
            Ellipse(hdc, ballX, ballY, ballX + 50, ballY + 50); // Ball dimensions
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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Move the ball
            moveBall();

            // Redraw the window
            InvalidateRect(hwnd, NULL, TRUE);

            // Control refresh rate (60 Hz)
            Sleep(16); // 16 milliseconds per frame (approx. 1000/60)
        }
    }

    return 0;
}
