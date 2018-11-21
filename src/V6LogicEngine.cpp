#include "..\\inc\\V6LogicEngine.h"
#include "..\\inc\\V6UtilityFuncLib.h"
#include "..\\inc\\V6RenderEngine.h"
namespace VoltaEngine{
	void LogicManager::Update(int t){
		for_each(Steps.begin(), Steps.end(), [&](LogicStep* step){
			step->Update(t, &Sprites, this);
		});
		for_each(Sprites.begin(), Sprites.end(), [&](LogicSprite* s){
			s->Sync();
		});
	}
	void LogicManager::Clear(){
		DisposeList(&Steps);
		DisposeList(&Sprites);
	}

	void VoltaLogicEngine::Update(int t){
		Manager.Update(t);
		g_time += t;
		//VoltaRenderEngine::CBufferContent.g_time = XMFLOAT4(g_time, 0.0f, 0.0f, 0.0f);
	}

	LogicManager VoltaLogicEngine::Manager; 
	int VoltaLogicEngine::g_time;
}