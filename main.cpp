#include <windows.h>
#include <gdiplus.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 700

#define BRICK_COLUMS 6
#define BRICK_ROWS 6
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define BRICK_GAP 20

#define BALL_SIZE 20

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 10
#define PADDLE_SPEED 10

void playGame(void);
void drawPaddle(void);
void drawBall(void);
// void exitGame(void);

const TCHAR ver[] = L"0.1-beta";
HWND hWnd;
RECT rBall;
int paddleX = 200, paddleY = WINDOW_HEIGHT - PADDLE_HEIGHT * 8;
int ballX = paddleX + 40, ballY = paddleY - BALL_SIZE;
int ballDx = 4, ballDy = 6;
int lLife = 3;

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

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
			drawPaddle();
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
//	drawBricks();
	drawPaddle();

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
	else if (ballY > (600))	{
		ballDy = -ballDy;
		ballY += ballDy;
		lLife--;
	}
	
	InvalidateRect(hWnd, &rBall, TRUE);
	drawBall();
}

void drawBall(void) {
	PAINTSTRUCT psBall;
	HDC hdc = BeginPaint(hWnd, &psBall);

	rBall.left = ballX;
	rBall.top = ballY;
	rBall.right = ballX + BALL_SIZE;
	rBall.bottom = ballY + BALL_SIZE;

	SelectObject(hdc, GetStockObject(WHITE_PEN));
	Rectangle(hdc, rBall.left, rBall.top, rBall.right, rBall.bottom);
	EndPaint(hWnd, &psBall);
	DeleteDC(hdc);
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

/*void exitGame(void) {
	MessageBox(NULL, L"exit msg", L"works", MB_OK);
	// clean trash after running
}*/
