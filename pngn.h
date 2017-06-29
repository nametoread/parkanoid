#pragma once

#include <windows.h>

///						defines - begin
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 700

#define BRICK_COLUMNS 6
#define BRICK_ROWS 6
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define BRICK_GAP 18

#define BALL_SIZE 20

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 10
#define PADDLE_SPEED 10
///						defines - end

///						var-n-const - begin
const TCHAR ver[] = L"0.1-release";
HWND hWnd;
RECT rBall, rPaddle, rBrick;
TCHAR bricks[BRICK_ROWS][BRICK_COLUMNS];

int paddleX = 200, paddleY = WINDOW_HEIGHT - PADDLE_HEIGHT * 8;
int ballX = paddleX + rand() % 10, ballY = paddleY - BALL_SIZE;
int ballDx = 4, ballDy = 6;
int lLife = 4, lHit = 0, lScore = 0;
///						var-n-const - end
