#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <algorithm>

struct Ball {
    float x, y;       // Position of the ball
    float velX, velY; // Velocity of the ball
    int size;         // Diameter of the ball
};

// Number of balls
const int BALL_COUNT = 10;

// Ball array
Ball balls[BALL_COUNT];

// Gravity constant
const float GRAVITY = 0.1f;

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void initializeBalls() {
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < BALL_COUNT; ++i) {
        bool overlap;
        do {
            overlap = false;
            balls[i].x = static_cast<float>(rand() % (WINDOW_WIDTH - 50) + 25);
            balls[i].y = static_cast<float>(rand() % (WINDOW_HEIGHT - 50) + 25);
            balls[i].size = 30;  // Fixed size
            balls[i].velX = static_cast<float>((rand() % 5) - 2); // Random velocity between -2 and 2
            balls[i].velY = static_cast<float>((rand() % 5) - 2);

            // Ensure no overlap between balls
            for (int j = 0; j < i; ++j) {
                float dx = balls[i].x - balls[j].x;
                float dy = balls[i].y - balls[j].y;
                float distance = sqrt(dx * dx + dy * dy);
                if (distance < balls[i].size) {
                    overlap = true;
                    break;
                }
            }
        } while (overlap);
    }
}

void moveBalls() {
    for (int i = 0; i < BALL_COUNT; ++i) {
        balls[i].velY += GRAVITY;  // Apply gravity

        // Update position based on velocity
        balls[i].x += balls[i].velX;
        balls[i].y += balls[i].velY;

        // Collision with the window's boundaries
        if (balls[i].x - balls[i].size / 2 < 0) {
            balls[i].x = balls[i].size / 2;
            balls[i].velX *= -1;
        }
        if (balls[i].x + balls[i].size / 2 > WINDOW_WIDTH) {
            balls[i].x = WINDOW_WIDTH - balls[i].size / 2;
            balls[i].velX *= -1;
        }
        if (balls[i].y - balls[i].size / 2 < 0) {
            balls[i].y = balls[i].size / 2;
            balls[i].velY *= -1;
        }
        if (balls[i].y + balls[i].size / 2 > WINDOW_HEIGHT) {
            balls[i].y = WINDOW_HEIGHT - balls[i].size / 2;
            balls[i].velY *= -1;
        }

        // Ball-to-ball collision
        for (int j = i + 1; j < BALL_COUNT; ++j) {
            float dx = balls[j].x - balls[i].x;
            float dy = balls[j].y - balls[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < balls[i].size) {
                // Resolve overlap
                float overlap = balls[i].size - distance;
                float nx = dx / distance;
                float ny = dy / distance;

                balls[i].x -= nx * overlap / 2;
                balls[i].y -= ny * overlap / 2;
                balls[j].x += nx * overlap / 2;
                balls[j].y += ny * overlap / 2;

                // Exchange velocities
                std::swap(balls[i].velX, balls[j].velX);
                std::swap(balls[i].velY, balls[j].velY);
            }
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Clear the background (black)
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrushBackground = CreateSolidBrush(RGB(0, 0, 0)); // Black background
            FillRect(hdc, &rect, hBrushBackground);
            DeleteObject(hBrushBackground);

            // Draw all balls
            for (int i = 0; i < BALL_COUNT; ++i) {
                HBRUSH hBrushBall = CreateSolidBrush(RGB(255, 0, 0)); // Red balls
                SelectObject(hdc, hBrushBall);
                Ellipse(hdc,
                    static_cast<int>(balls[i].x - balls[i].size / 2),
                    static_cast<int>(balls[i].y - balls[i].size / 2),
                    static_cast<int>(balls[i].x + balls[i].size / 2),
                    static_cast<int>(balls[i].y + balls[i].size / 2));
                DeleteObject(hBrushBall);
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BallSimulation";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Ball Simulation", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    // Initialize balls with random positions and velocities
    initializeBalls();

    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Move the balls
            moveBalls();

            // Redraw the window
            InvalidateRect(hwnd, NULL, TRUE);

            // Control refresh rate (120 FPS)
            Sleep(8);  // 8 milliseconds per frame (approx. 120 FPS)
        }
    }

    return 0;
}
