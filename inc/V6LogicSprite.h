#pragma once
#include "stdafx.h"

class Sprite;
namespace VoltaEngine{
	class LogicSprite{
	public:
		LogicSprite(){ FillEvents(); }
		// ������ľ���ͬ����
		virtual void Sync() = 0;

		// �¼�ע���
		map<string, void(*)(int, int, int)> Events;
		Sprite* RelatedSprite;
	protected:
		inline void FillEvents(){

		}
	};
}