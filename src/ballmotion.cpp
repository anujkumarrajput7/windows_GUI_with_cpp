#include <windows.h>
#include <math.h> // Include for sin and cos functions

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void motion(HWND hwnd);

// Global variables for the ball's position and circular motion
float angle = 0.0f; // Angle in radians
const float radius = 100.0f; // Radius of the circular path
int ballX = 0; // Initial x position of the ball
int ballY = 0; // Initial y position of the ball
int centerX = 150; // Center x position of the circle
int centerY = 150; // Center y position of the circle

// This function handles the motion of the ball
void motion(HWND hwnd) {
    // Update the angle
    angle += 0.1f; // Increment the angle (adjust speed of rotation)

    // Calculate the ball's position in circular motion
    ballX = centerX + (int)(radius * cos(angle)) - 50; // Subtract half the ball's width
    ballY = centerY + (int)(radius * sin(angle)) - 50; // Subtract half the ball's height

    // Redraw the window
    InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
}

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
            HBRUSH hBrush = CreateSolidBrush(RGB(173, 216, 230)); // Light blue color
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
            Ellipse(hdc, ballX, ballY, ballX + 100, ballY + 100); // x1, y1, x2, y2

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