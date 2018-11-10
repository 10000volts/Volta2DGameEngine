#pragma once

#include "stdafx.h"
#include "V6LogicStep.h"
#include "V6LogicSprite.h"

namespace VoltaEngine{
	class LogicManager{
	public:
		void Update(int);
		void Clear();
		inline void AddSprite(LogicSprite* s){ Sprites.push_back(s); }
		inline void AddStep(LogicStep* s){ Steps.push_back(s); }
		inline void RemoveSprite(LogicSprite* s){ Sprites.remove(s); delete s; s = 0; }
		inline void RemoveStep(LogicStep* s){ Steps.remove(s); delete s; s = 0; }
	protected:
		list<LogicSprite*> Sprites;
		list<LogicStep*> Steps;
	};
	class VoltaLogicEngine{
	public:
		static inline void Update(int t){
			Manager.Update(t);
		}
		static void Dispose(){
			Manager.Clear();
		}
		// 其实应该作封装。
		static LogicManager Manager;
	};
}