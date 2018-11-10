#pragma once

#include "V6RenderEngine.h"
#include "V6LogicEngine.h"
#include "V6ResourceDictionary.h"
namespace VoltaEngine{
	class RenderForm{
	public:
		// 创建作为渲染引擎载体的窗体。同时默认装载逻辑引擎。返回0说明成功。
		static int Create(HINSTANCE hInstance, HWND& hw);
		static int TakeOver(HWND hw);
	protected:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}