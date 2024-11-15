#include <windows.h>
#include <math.h> 

// Structure for the physics entity
struct PhysicsEntity {
    float posX;
    float posY;
    float velocityX;
    float velocityY;
    float accelerationX;
    float accelerationY;
    float mass;
};

// Update physics function based on deltaTime
void updatePhysics(PhysicsEntity& entity, float deltaTime) {
    // Update velocity based on acceleration
    entity.velocityX += entity.accelerationX * deltaTime;
    entity.velocityY += entity.accelerationY * deltaTime;

    // Update position based on velocity
    entity.posX += entity.velocityX * deltaTime;
    entity.posY += entity.velocityY * deltaTime;

    // Optional: Implement custom bounds or collision detection
    if (entity.posX < 0 || entity.posX > 300) entity.velocityX *= -1;
    if (entity.posY < 0 || entity.posY > 300) entity.velocityY *= -1;
}

// Global entity representing the ball
PhysicsEntity ball = {150, 150, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

// Function to handle motion based on physics engine updates
void motion(HWND hwnd, PhysicsEntity& entity, float deltaTime) {
    // Update physics
    updatePhysics(entity, deltaTime);

    // Request a redraw
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

            // Draw the ball based on its updated position
            Ellipse(hdc, (int)ball.posX, (int)ball.posY, (int)ball.posX + 20, (int)ball.posY + 20);

            DeleteObject(hBrush);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "PhysicsWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Physics Simulation", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    float deltaTime = 0.016f; // Assuming ~60 frames per second
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Update and render the physics-based motion of the entity
        motion(hwnd, ball, deltaTime);

        Sleep(16); // 16 ms sleep for ~60 FPS
    }

    return 0;
}
