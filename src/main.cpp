#include "..\\inc\\V6RenderForm.h"

using namespace VoltaEngine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR, int){
	HWND hw;
	if (!RenderForm::Create(hInstance, hw, V6WINDOWWIDTH, V6WINDOWHEIGHT, true)){

		TexSprite* s = new TexSprite("image1", 450.0f, 360.0f);
		s->angle_ = 0.4f;
		VoltaRenderEngine::Manager.AddSprite(s);

		s = new TexSprite("image1", 600.0f, 460.0f);
		s->highlight_ = true;
		VoltaRenderEngine::Manager.AddSprite(s);

		s = new TexSprite("image1", 700.0f, 300.0f);
		VoltaRenderEngine::Manager.AddSprite(s);

		ShaderSprite* ss;

		ss = new ShaderSprite("Blur", 512.0f, 384.0f, 1024.0f, 768.0f, 0.5f, "CLAMP");
		VoltaRenderEngine::Manager.AddScreenShader(ss); 

		// 注释下面的这两行是模糊->清晰的效果，保留这两行是清晰->模糊的效果。
		ss = new ShaderSprite("Empty", 512.0f, 384.0f, 1024.0f, 768.0f, 0.5f, "CLAMP");
		VoltaRenderEngine::Manager.AddScreenShader(ss);

		ss = new ShaderSprite("FadeOut", 0.0f, 0.0f, 1024.0f, 768.0f, 0.5f, "CLAMP");
		VoltaRenderEngine::Manager.AddScreenShader(ss);
		SimpleLSprite* sls = new SimpleLSprite(ss, 512.0f, 384.0f);
		VoltaLogicEngine::Manager.AddSprite(sls);
		ss->StartAnimation(2000);

		return RenderForm::TakeOver(hw);
	}
	return 0;
}