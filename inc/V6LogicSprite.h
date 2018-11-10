#pragma once
#include "stdafx.h"

class Sprite;
namespace VoltaEngine{
	class LogicSprite{
	public:
		LogicSprite(){ FillEvents(); }
		// 与关联的精灵同步。
		virtual void Sync() = 0;

		// 事件注册表。
		map<string, void(*)(int, int, int)> Events;
		Sprite* RelatedSprite;
	protected:
		inline void FillEvents(){

		}
	};
}