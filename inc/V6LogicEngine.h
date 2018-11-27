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
		inline void RemoveSprite(LogicSprite* s){ Sprites.remove(s); delete s; s = nullptr; }
		inline void RemoveStep(LogicStep* s){ Steps.remove(s); delete s; s = nullptr; }
	protected:
		list<LogicSprite*> Sprites;
		list<LogicStep*> Steps;
	};
	class VoltaLogicEngine{
	public:
		static void Update(int t);
		inline static void Dispose(){
			Manager.Clear();
		}
		// 其实应该作封装。
		static LogicManager Manager;
		// 全局时间。
		static long long g_time;
	};
}