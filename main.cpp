#include <windows.h>

TCHAR ver[] = L"0.1-alfa";

LRESULT CALLBACK wndProcess(HWND hWnd, UINT uMssg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	TCHAR ctrlClass[] = L"Control Class";
	WNDCLASSEX wndClass;
	HWND hWnd;
	MSG mMssg;

	ZeroMemory(&wndClass, sizeof(wndClass));

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

	if (!RegisterClassEx(&wndClass)) {			// class check
		MessageBox(NULL, L"code: 10", L"error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	hWnd = CreateWindow(ctrlClass, L"parkanoid", NULL,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 700, NULL, NULL, HINSTANCE(hInst), NULL);

	if (!hWnd) {								// window check
		MessageBox(NULL, L"code: 20", L"error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&mMssg, NULL, NULL, NULL)) {
		TranslateMessage(&mMssg);
		DispatchMessage(&mMssg);
	}

	return NULL;
}

LRESULT CALLBACK wndProcess(HWND hWnd, UINT uMssg, WPARAM wParam, LPARAM lParam) {
	switch (uMssg) {
		case WM_CREATE:				// intro alert
/*			MessageBox(NULL, L"controls:\nmove\tleft/right\nstart\tspace\nexit\tesc",
			L"welcome to parkanoid!", MB_ICONINFORMATION);
*/			break;
		case WM_KEYDOWN:			// key pressed
			switch (wParam) {
				case VK_SPACE:			// space
					// start
					break;
				case VK_ESCAPE:			// esc
					DestroyWindow(hWnd);
					break;
				case VK_LEFT:			// left
					// move
					break;
				case VK_RIGHT:			// right
					// move
					break;
				case VK_APPS:			// menu
					// to be continued
					break;
				default:				// def
					break;
			}
			break;
		case WM_CLOSE:				// destroy
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:			// terminate
/*			MessageBox(NULL, L"well played", L"exit", MB_OK);
*/			PostQuitMessage(NULL);
			break;
		default:					// def
			return DefWindowProc(hWnd, uMssg, wParam, lParam);
	}
	
	return NULL;
}
