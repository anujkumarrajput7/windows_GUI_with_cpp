#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void motion(HWND hwnd);

// Global variables for the ball's position and vertical line motion
int ballX = 150;               // Fixed x position of the ball (center of the window)
int ballY = 100;                 // Initial y position of the ball
int centerY = 150;             // Center y position for vertical motion
int maxOffset = 100;           // Maximum distance the ball moves up and down
int direction = 1;             // 1 = moving down, -1 = moving up

// This function handles the vertical line motion of the ball
void motion(HWND hwnd) {
    // Update the ball's y position for vertical line motion
    ballY += direction * 2;    // Adjust the speed by changing the increment
    // Reverse direction when reaching the vertical limits
    if (ballY >= centerY + maxOffset || ballY <= centerY - maxOffset) {
        direction *= -1;
    }

    // Redraw the window
    InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rect;
            GetClientRect(hwnd, &rect);

            // Set the background color (e.g., light blue)
            HBRUSH hBrush = CreateSolidBrush(RGB(173, 216, 230)); // Light blue color
            FillRect(hdc, &rect, hBrush);

            DeleteObject(hBrush); // Clean up the brush
            return 1;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set the color for the ball
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Red color
            SelectObject(hdc, hBrush);

            // Draw the ball (an ellipse)
            Ellipse(hdc, ballX - 50, ballY - 50, ballX + 50, ballY + 50);

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
        
        // Call the motion function to update the ball's position
        motion(hwnd);
        
        // Sleep for a short time to control the animation speed
        Sleep(20); // Adjust the speed of the motion
    }

    return 0;
}
