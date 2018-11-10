#pragma once

#include "stdafx.h"
#include "V6LogicSprite.h"

namespace VoltaEngine{
	class LogicManager;
	// �߼��������ʱ��ÿһ�����衣
	class LogicStep{
	public:
		LogicStep(void(*s)(int t, list<LogicSprite*>*, LogicManager*)) : step(s){}
		inline void Update(int t, list<LogicSprite*>* l, LogicManager* lm){
			step(t, l, lm);
		}
	protected:
		void(*step)(int, list<LogicSprite*>*, LogicManager*);
	};
}