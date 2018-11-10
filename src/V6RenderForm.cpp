#include "..\\inc\\V6RenderForm.h"
namespace VoltaEngine{
	int RenderForm::Create(HINSTANCE hInstance, HWND& hw){
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = V6GAMETITLE;

		if (!RegisterClassEx(&wndClass))
			return -1;

		hw = CreateWindowA(V6GAMETITLE, V6GAMETITLE, V6WS_WINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, V6WINDOWWIDTH, V6WINDOWHEIGHT,
			NULL, NULL, hInstance, NULL);

		if (!hw)
			return -1;

		if (!VoltaEngine::Init(hInstance, hw))
			return -1;

		return 0;
	}
	int RenderForm::TakeOver(HWND hw){
		ShowWindow(hw, V6SW_WINDOW);

		MSG msg = { 0 };

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			VoltaLogicEngine::Update(0);
			VoltaRenderEngine::Render();
		}

		VoltaRenderEngine::Dispose();

		return static_cast<int>(msg.wParam);
	}
	LRESULT CALLBACK RenderForm::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
		PAINTSTRUCT paintStruct;
		HDC hDC;

		switch (message)
		{
		case WM_PAINT:
			hDC = BeginPaint(hwnd, &paintStruct);
			EndPaint(hwnd, &paintStruct);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		return 0;
	}
}