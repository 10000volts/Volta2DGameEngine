#include "..\\inc\\V6LogicEngine.h"
#include "..\\inc\\V6UtilityFuncLib.h"
#include "..\\inc\\V6RenderEngine.h"

namespace VoltaEngine{
	void LogicManager::Update(int t){
		for_each(Steps.begin(), Steps.end(), [&t, this](LogicStep* step){
			step->Update(t, &Sprites, this);
		});
		for_each(Sprites.begin(), Sprites.end(), [&t, this](LogicSprite* s){
			s->Update(t);
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
		VoltaRenderEngine::CBPerFContent_.g_time = XMFLOAT4(static_cast<float>(g_time), 0.0f, 0.0f, 0.0f);
	}

	LogicManager VoltaLogicEngine::Manager; 
	long long VoltaLogicEngine::g_time;
}