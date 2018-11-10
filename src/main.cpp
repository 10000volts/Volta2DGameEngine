#include "..\\inc\\V6RenderForm.h"

using namespace VoltaEngine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR, int){
	HWND hw;
	if (!RenderForm::Create(hInstance, hw)){

		TexSprite* s = new TexSprite("image1", 50, 100, 400, 400);
		VoltaRenderEngine::Manager.AddSprite(s);

		return RenderForm::TakeOver(hw);
	}
	return 0;
}