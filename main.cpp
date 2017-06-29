#include <windows.h>
#include <gdiplus.h>

#define dbg MessageBox(NULL, L"test", L"here", MB_OK)

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

void playGame(void);
void drawBrick(void);
void drawPaddle(void);
void drawBall(void);
bool collisionPaddle();
void collisionBrick();
// void exitGame(void);

const TCHAR ver[] = L"0.3-beta";
HWND hWnd;
RECT rBall, scrn;
TCHAR bricks[BRICK_ROWS][BRICK_COLUMNS];
int paddleX = 200, paddleY = WINDOW_HEIGHT - PADDLE_HEIGHT * 8;
int ballX = paddleX + 40, ballY = paddleY - BALL_SIZE;
int ballDx = 4, ballDy = 6;
int lLife = 3, lHit = 0, lScore = 0;

LRESULT CALLBACK wndProcess(HWND hWnd, UINT uMssg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	const TCHAR ctrlClass[] = L"Control Class";
	WNDCLASSEX wndClass;
	MSG mMssg;

	ZeroMemory(&wndClass, sizeof(wndClass));
// class description - begin
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = NULL;
	wndClass.lpfnWndProc = wndProcess;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = ctrlClass;
	wndClass.cbWndExtra = NULL;
	wndClass.cbClsExtra = NULL;
	wndClass.hIcon = LoadIcon(NULL, IDI_SHIELD);
	wndClass.hIconSm = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hInstance = hInst;
// class description - end

	if (!RegisterClassEx(&wndClass)) {			// class check
		MessageBox(NULL, L"code: #1", L"error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	hWnd = CreateWindow(ctrlClass, L"parkanoid", NULL,
		50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, HINSTANCE(hInst), NULL);

	if (!hWnd) {								// window check
		MessageBox(NULL, L"code: #2", L"error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	scrn.left = 1;
	scrn.top = 1;
	scrn.right = 499;
	scrn.bottom = 699;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	for (int row = 0; row < BRICK_ROWS; row++)
		for (int col = 0; col < BRICK_COLUMNS; col++)
			bricks[row][col] = row * 6 + col * 6 + 20;

	while(1) {
		if (PeekMessage(&mMssg, NULL, NULL, NULL, PM_REMOVE)) {
			if (mMssg.message == WM_QUIT) break;
			
			TranslateMessage(&mMssg);
			DispatchMessage(&mMssg);
		}
		playGame();
	}

//	exitGame();
	return mMssg.wParam;
}

// process - begin
LRESULT CALLBACK wndProcess(HWND hWnd, UINT uMssg, WPARAM wParam, LPARAM lParam) {
	switch (uMssg) {
		case WM_CREATE:			// intro alert
/*			MessageBox(NULL, L"controls:\nmove\tleft/right\nstart\tspace\nexit\tesc",
			L"welcome to parkanoid!", MB_ICONINFORMATION);
*/
			break;
		case WM_PAINT:
			break;
		case WM_KEYDOWN:		// key press
			switch (wParam) {
				case VK_SPACE:			// space
					break;
				case VK_ESCAPE:			// esc
					DestroyWindow(hWnd);
					break;
				case VK_LEFT:			// left
					paddleX -= PADDLE_SPEED;
					if (paddleX < 0) paddleX = 0;
					else InvalidateRect(hWnd, NULL, TRUE);
					break;
				case VK_RIGHT:			// right
					paddleX += PADDLE_SPEED;
					if (paddleX > (WINDOW_WIDTH - PADDLE_WIDTH))
						paddleX = WINDOW_WIDTH - PADDLE_WIDTH;
					else InvalidateRect(hWnd, NULL, TRUE);
					break;
				case VK_APPS:			// menu
					MessageBox(NULL, L"CH34T", L"4L3RT", MB_ICONEXCLAMATION);
					break;
				default:				// def
					break;
			}
			break;
		case WM_CLOSE:			// destroy
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:		// terminate
/*			MessageBox(NULL, L"well played", L"exit", MB_OK);
*/			PostQuitMessage(NULL);
			break;
		default:				// def
			return DefWindowProc(hWnd, uMssg, wParam, lParam);
	}

	return NULL;
} // process - end

void playGame(void) {
	//drawPaddle();
	drawBall();
	ballX += ballDx;
	ballY += ballDy;

	if (ballX > (WINDOW_WIDTH - BALL_SIZE*2) || ballX < 0) {
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
	}
	else if ((ballY > 600) && collisionPaddle()) {
		ballDy = -ballDy;
		ballY += ballDy;
	}
	
	InvalidateRect(hWnd, &rBall, TRUE);
	collisionBrick();
	
	drawBrick();
}

void drawBrick(void) {
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
	DeleteDC(hdc);
}

void drawBall(void) {
	PAINTSTRUCT psBall;
	HDC hdcBall = BeginPaint(hWnd, &psBall);

	rBall.left = ballX;
	rBall.top = ballY;
	rBall.right = ballX + BALL_SIZE;
	rBall.bottom = ballY + BALL_SIZE;

	SelectObject(hdcBall, GetStockObject(WHITE_PEN));
	Rectangle(hdcBall, rBall.left, rBall.top, rBall.right, rBall.bottom);
	EndPaint(hWnd, &psBall);
	DeleteDC(hdcBall);
	Sleep(66);
}

void drawPaddle(void) {
	PAINTSTRUCT ps;
	HGDIOBJ objPaddle = NULL;
	HDC hdc = BeginPaint(hWnd, &ps);
	int x1 = paddleX,
		y1 = WINDOW_HEIGHT - PADDLE_HEIGHT * 8,
		x2 = x1 + PADDLE_WIDTH,
		y2 = y1 + PADDLE_HEIGHT;

	objPaddle = SelectObject(hdc, GetStockObject(WHITE_PEN));
	Rectangle(hdc, x1, y1, x2, y2);
	EndPaint(hWnd, &ps);
	DeleteDC(hdc);
}

bool collisionPaddle() {
	return ((rBall.left > paddleX - 5) && (rBall.left < paddleX + PADDLE_WIDTH + 5));
}

void collisionBrick(void) {
	int ballCx = ballX + (BALL_SIZE / 2),
		ballCy = ballY + (BALL_SIZE / 2),
		x1 = BRICK_GAP,
		y1 = BRICK_GAP;

	if (lHit >= (BRICK_ROWS * BRICK_COLUMNS)) {
		// win
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
					ballDx += 2;
					lScore += 10;
					return;
				}
			} x1 += BRICK_GAP;
		} y1 += BRICK_GAP;
	};
}

/*void exitGame(void) {
	MessageBox(NULL, L"exit msg", L"works", MB_OK);
	// clean trash after running
}*/
