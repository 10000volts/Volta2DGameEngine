#pragma once

#include "V6RenderEngine.h"
#include "V6LogicEngine.h"
#include "V6ResourceDictionary.h"
namespace VoltaEngine{
	class RenderForm{
	public:
		// ������Ϊ��Ⱦ��������Ĵ��塣ͬʱĬ��װ���߼����档����0˵���ɹ���
		static int Create(HINSTANCE hInstance, HWND& hw);
		static int TakeOver(HWND hw);
	protected:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}