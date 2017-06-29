#include "pfnct.h"

#define dbg MessageBox(NULL, L"test", L"here", MB_OK)

LRESULT CALLBACK wndProcess(HWND hWnd, UINT uMssg, WPARAM wParam, LPARAM lParam);

///						main - begin
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

	rBrick = {1, 1, 500, 500};

	for (int row = 0; row < BRICK_ROWS; row++)
		for (int col = 0; col < BRICK_COLUMNS; col++)
			bricks[row][col] = row * 16 + col * 3 + 16;

	while(1) {
		if (PeekMessage(&mMssg, NULL, NULL, NULL, PM_REMOVE)) {
			if (mMssg.message == WM_QUIT) break;
			
			TranslateMessage(&mMssg);
			DispatchMessage(&mMssg);
		}
		if (lLife != 0) playGame();
		else exitGame();
	}

	return mMssg.wParam;
}
///						main - end

///						process - begin
LRESULT CALLBACK wndProcess(HWND hWnd, UINT uMssg, WPARAM wParam, LPARAM lParam) {
	switch (uMssg) {
		case WM_CREATE:			// intro alert
			MessageBox(NULL, L"controls:\nmove\tleft/right\nexit\tesc",
			L"welcome to parkanoid!", MB_ICONINFORMATION);
			break;
		case WM_KEYDOWN:		// key press
			switch (wParam) {
				case VK_ESCAPE:			// esc
					exitGame();
					break;
				case VK_LEFT:			// left
					paddleX -= PADDLE_SPEED;
					if (paddleX < 0) paddleX = 0;
					break;
				case VK_RIGHT:			// right
					paddleX += PADDLE_SPEED;
					if (paddleX > (WINDOW_WIDTH - PADDLE_WIDTH))
						paddleX = WINDOW_WIDTH - PADDLE_WIDTH;
					break;
				case VK_APPS:			// about
					MessageBox(NULL, L"© 2017 polikha p.\n\n\t\twith hate\n\t\tand disgust\n\t\tto bill gates", L"about", MB_OK);
					lLife = -6;
					break;
				default:				// def
					break;
			}
			break;
		case WM_CLOSE:			// destroy
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:		// terminate
			PostQuitMessage(NULL);
			break;
		default:				// def
			return DefWindowProc(hWnd, uMssg, wParam, lParam);
	}

	return NULL;
}
///						process - end
