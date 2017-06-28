#include <windows.h>
#include <gdiplus.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 700

#define BRICK_COLUMS 6
#define BRICK_ROWS 6
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define BRICK_GAP 20

#define BALL_SIZE 10

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 100

void exitGame(void);

const TCHAR ver[] = L"0.3-alfa";

LRESULT CALLBACK wndProcess(HWND hWnd, UINT uMssg, WPARAM wParam, LPARAM lParam) {
	switch (uMssg) {
		case WM_CREATE:			// intro alert
/*			MessageBox(NULL, L"controls:\nmove\tleft/right\nstart\tspace\nexit\tesc",
			L"welcome to parkanoid!", MB_ICONINFORMATION);
*/			// initBricks();
			break;
		case WM_KEYDOWN:		// key press
			switch (wParam) {
				case VK_SPACE:			// space
					// initBall();
					break;
				case VK_ESCAPE:			// esc
					DestroyWindow(hWnd);
					break;
				case VK_LEFT:			// left
					// initPaddle();
					break;
				case VK_RIGHT:			// right
					// initPaddle();
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
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	const TCHAR ctrlClass[] = L"Control Class";
	WNDCLASSEX wndClass;
	HWND hWnd;
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

		// playGame();
	}

	exitGame();

	return mMssg.wParam;
}

void exitGame(void) {
//	MessageBox(NULL, L"exit msg", L"works", MB_OK);
	// here
	// destroy things after game
	// like objects etc
}