#include "..\\inc\\V6LogicEngine.h"
#include "..\\inc\\V6UtilityFuncLib.h"
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

	LogicManager VoltaLogicEngine::Manager;
}