#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void motion(HWND hwnd);

// Global variables for the ball's position and square motion
int ballX = 0;                   // Initial x position of the ball
int ballY = 0;                   // Initial y position of the ball
int centerX = 150;               // Starting x position for the square path
int centerY = 150;               // Starting y position for the square path
int sideLength = 100;            // Side length of the square path
int step = 0;                    // Step for square motion direction

// This function handles the square motion of the ball
void motion(HWND hwnd) {
    // Move the ball in a square path
    switch (step) {
        case 0: // Move right
            ballX += 2;
            if (ballX >= centerX + sideLength) step = 1;
            break;
        case 1: // Move up
            ballY -= 2;
            if (ballY <= centerY - sideLength) step = 2;
            break;
        case 2: // Move left
            ballX -= 2;
            if (ballX <= centerX) step = 3;
            break;
        case 3: // Move down
            ballY += 2;
            if (ballY >= centerY) step = 0;
            break;
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
            Ellipse(hdc, ballX, ballY, ballX + 100, ballY + 100);

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
