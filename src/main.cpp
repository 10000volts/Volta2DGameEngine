#include "..\\inc\\V6RenderForm.h"

using namespace VoltaEngine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR, int){
	HWND hw;
	if (!RenderForm::Create(hInstance, hw, V6WINDOWWIDTH, V6WINDOWHEIGHT, true)){

		VoltaLogicEngine::Manager.AddStep("CheckHovering");

		string op[] = {"gamestart", "settings", "quitgame"};
		SelectBoard* sb = new SelectBoard(op, "underline", 50.0f, 100.0f, 3);
		VoltaRenderEngine::Manager.AddSprite(sb);
		LSelectBoard* lsb = new LSelectBoard(sb, 700.0f, 500.0f, 50.0f, 200.0f, 100.0f, 40.0f, 3);
		VoltaLogicEngine::Manager.AddControl(lsb);
		VoltaLogicEngine::Manager.focus_control_ = lsb;

		return RenderForm::TakeOver();
	}
	return 0;
}