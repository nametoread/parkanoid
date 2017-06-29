#pragma once

#include "pngn.h"

///						proto - begin
void playGame(void);
void drawBrick(void);
void drawPaddle(void);
void drawBall(void);
bool collisionPaddle();
void collisionBrick();
// void exitGame(void);
///						proto - end


void playGame(void) {
	drawPaddle();
	
	ballX += ballDx;
	ballY += ballDy;

	drawBrick();

	if (ballX > (WINDOW_WIDTH - BALL_SIZE * 2) || ballX < 0) {
		ballDx = -ballDx;
		ballX += ballDx;
	}
	if (ballY < 0) {
		ballDy = -ballDy;
		ballY += ballDy;
	}
	else if ((ballY > 600) && !collisionPaddle()) {
		ballDy = -ballDy;
		ballY += ballDy;
		lLife--;
		MessageBeep(-1);
	}
	else if ((ballY > 600) && collisionPaddle()) {
		ballDy = -ballDy;
		ballY += ballDy;
	}

	drawBall();
	drawPaddle();
	
	InvalidateRect(hWnd, &rBall, TRUE);
	InvalidateRect(hWnd, &rPaddle, TRUE);

	Sleep(33);
}

void drawBrick(void) {
	InvalidateRect(hWnd, &rBrick, TRUE);
	int x1, y1 = BRICK_GAP;
	PAINTSTRUCT psBrick;
	HDC hdc = BeginPaint(hWnd, &psBrick);
	for (int row = 0; row < BRICK_ROWS; row++) {
		x1 = BRICK_GAP;
		for (int col = 0; col < BRICK_COLUMNS; col++) {
			if (bricks[row][col] != 0) {
				SelectObject(hdc, GetStockObject(WHITE_PEN));
				bricks[row][col] = Rectangle(hdc, x1, y1, x1 + BRICK_WIDTH, y1 + BRICK_HEIGHT);
			}
			x1 += BRICK_WIDTH + BRICK_GAP;
		}
		y1 += BRICK_HEIGHT + BRICK_GAP;
	}
	EndPaint(hWnd, &psBrick);
	//DeleteDC(hdc);
}

void drawBall(void) {
	collisionBrick();
	InvalidateRect(hWnd, &rBall, TRUE);
	PAINTSTRUCT psBall;
	HGDIOBJ objBall = NULL;
	HDC hdcBall = BeginPaint(hWnd, &psBall);
	rBall = {ballX, ballY, ballX + BALL_SIZE, ballY + BALL_SIZE};
	objBall = SelectObject(hdcBall, GetStockObject(WHITE_PEN));
	Rectangle(hdcBall, rBall.left, rBall.top, rBall.right, rBall.bottom);
	EndPaint(hWnd, &psBall);
	//DeleteDC(hdcBall);
}

void drawPaddle(void) {
	InvalidateRect(hWnd, &rPaddle, TRUE);
	PAINTSTRUCT ps;
	HGDIOBJ objPaddle = NULL;
	HDC hdc = BeginPaint(hWnd, &ps);
	rPaddle = {paddleX, WINDOW_HEIGHT - PADDLE_HEIGHT * 8,
		rPaddle.left + PADDLE_WIDTH, rPaddle.top + PADDLE_HEIGHT};
	objPaddle = SelectObject(hdc, GetStockObject(WHITE_PEN));
	Rectangle(hdc, rPaddle.left, rPaddle.top, rPaddle.right, rPaddle.bottom);
	EndPaint(hWnd, &ps);
	//DeleteDC(hdc);
}

bool collisionPaddle() {
	return ((rBall.left > paddleX - 9) && (rBall.left < paddleX + PADDLE_WIDTH + 9));
}

void collisionBrick(void) {
	int ballCx = ballX + (BALL_SIZE / 2),
		ballCy = ballY + (BALL_SIZE / 2),
		x1 = BRICK_GAP,
		y1 = BRICK_GAP;

	if (lHit >= (BRICK_ROWS * BRICK_COLUMNS)) {
		MessageBox(hWnd, L"huh, you win", (LPCWSTR)lScore, MB_OK);
		DestroyWindow(hWnd);
	}

	for (int row = 0; row < BRICK_ROWS; row++) {
		x1 = BRICK_GAP;
		for (int col = 0; col < BRICK_COLUMNS; col++) {
			if (bricks[row][col] != 0) {
				if ((ballCx > x1) && (ballCx < x1 + BRICK_WIDTH) &&
					(ballCy > y1) && (ballCy < y1 + BRICK_HEIGHT)) {
					bricks[row][col] = 0;
					lHit++;
					ballDy = -ballDy;
					ballDx = ((ballDx + 2) % 6) + 3;
					lScore += 10;
					return;
				}
			} x1 += BRICK_GAP;
		} y1 += BRICK_GAP;
	};
}

/*void exitGame(void) {
MessageBox(NULL, L"exit msg", L"works", MB_OK);
}*/