#include "..\\inc\\V6RenderForm.h"

using namespace VoltaEngine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR, int){
	HWND hw;
	if (!RenderForm::Create(hInstance, hw, 1024, 768, true)){

		TexSprite* s = new TexSprite("image1", 450.0f, 360.0f);
		s->Angle = 0.4f;
		VoltaRenderEngine::Manager.AddSprite(s);

		s = new TexSprite("image1", 600.0f, 460.0f);
		s->Highlight = true;
		VoltaRenderEngine::Manager.AddSprite(s);

		s = new TexSprite("image1", 700.0f, 300.0f, 1.0f, 1.0f, 0.5f, "Blur", "CLAMP");
		VoltaRenderEngine::Manager.AddSprite(s);

		ShaderSprite* ss = new ShaderSprite("Blur", 512.0f, 384.0f, 1024.0f, 768.0f, 0.5f, "CLAMP");
		VoltaRenderEngine::Manager.AddScreenShader(ss);

		return RenderForm::TakeOver(hw);
	}
	return 0;
}