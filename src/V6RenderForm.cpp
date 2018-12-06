#include <Windows.h>
#include "..\\inc\\V6RenderForm.h"

namespace VoltaEngine{
	int RenderForm::Create(HINSTANCE hInstance, HWND& hw, int width, int height, bool window){
		RECT rc = { 0, 0, width, height };
		if(window) AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		Window = window;
		
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
		long long flag = window ? V6WS_WINDOW : V6WS_FULLSCREEN;
		hw = CreateWindowA(V6GAMETITLE, V6GAMETITLE, flag,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
			NULL, NULL, hInstance, NULL);

		if (!hw)
			return -1;

		if (!VoltaEngine::Init(hInstance, hw, width, height, window))
			return -1;

		m_hwnd_ = hw;

		return 0;
	}
	int RenderForm::TakeOver(){
		if (Window){
			ShowWindow(m_hwnd_, V6SW_WINDOW);
		}
		else{
			ShowWindow(m_hwnd_, V6SW_FULLSCREEN);
		}

		MSG msg = { 0 };

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			timeBeginPeriod(1);
			static DWORD fPreTime = (timeGetTime());
			DWORD fCurrentTime = (timeGetTime());
			int fElapsedTime = static_cast<int>(fCurrentTime - fPreTime);
			timeEndPeriod(1);

			UpdateMousePos();
			VoltaLogicEngine::Update(fElapsedTime);
			VoltaRenderEngine::Render();

			fPreTime = fCurrentTime;
		}

		VoltaEngine::Unload();

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
			VoltaLogicEngine::Handle(hwnd, message, wParam, lParam);
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		return 0;
	}
	void RenderForm::UpdateMousePos(){
		GetCursorPos(&mouse_pos_);
		ScreenToClient(m_hwnd_, &mouse_pos_);
		mouse_pos_.y = VoltaRenderEngine::render_height_ - mouse_pos_.y;
	}

	HWND RenderForm::m_hwnd_;
	bool RenderForm::Window;
	POINT RenderForm::mouse_pos_;
}