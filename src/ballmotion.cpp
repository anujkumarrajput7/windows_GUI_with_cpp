#include <windows.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void motion(HWND hwnd, float deltaTime);

// Struct for storing physics-related data
struct PhysicsData {
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    float accelerationX;
    float accelerationY;
};

// Initialize the physics data for the ball
PhysicsData ballPhysics = {150.0f, 150.0f, 0.0f, 0.0f, 0.0f, 0.0f};

// Global variables for the ball’s circular motion
float angle = 0.0f;
const float radius = 100.0f;
int ballX = 0;
int ballY = 0;
int centerX = 150;
int centerY = 150;

// Simulates the physics for the ball and updates its position
void simulatePhysics(PhysicsData& physics, float deltaTime) {
    // Update velocity based on acceleration
    physics.velocityX += physics.accelerationX * deltaTime;
    physics.velocityY += physics.accelerationY * deltaTime;

    // Update position based on velocity
    physics.positionX += physics.velocityX * deltaTime;
    physics.positionY += physics.velocityY * deltaTime;
}

// This function handles the motion of the ball using physics data
void motion(HWND hwnd, float deltaTime) {
    simulatePhysics(ballPhysics, deltaTime);

    // Apply circular motion by modifying x and y based on angle
    angle += 0.1f * deltaTime;
    ballX = (int)(ballPhysics.positionX + radius * cos(angle) - 50);
    ballY = (int)(ballPhysics.positionY + radius * sin(angle) - 50);

    InvalidateRect(hwnd, NULL, TRUE);
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
            HBRUSH hBrush = CreateSolidBrush(RGB(173, 216, 230));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, hBrush);

            Ellipse(hdc, ballX, ballY, ballX + 100, ballY + 100);

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
        0, CLASS_NAME, "Ball with Physics Engine", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    float deltaTime = 0.02f;  // Time between frames
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        motion(hwnd, deltaTime);

        Sleep(20);
    }

    return 0;
}
