#pragma once

#include "V6RenderEngine.h"
#include "V6LogicEngine.h"
#include "V6ResourceDictionary.h"

namespace VoltaEngine{
	class RenderForm{
	public:
		// ������Ϊ��Ⱦ��������Ĵ��塣ͬʱĬ��װ���߼����档����0˵���ɹ���
		static int Create(HINSTANCE hInstance, HWND& hw, int width, int height, bool window);
		static int TakeOver();
		inline static void Close(){
			PostQuitMessage(0);
		}

		static POINT mouse_pos_;
	protected:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		static void UpdateMousePos();

		static HWND m_hwnd_;
		static bool Window;
	};
}