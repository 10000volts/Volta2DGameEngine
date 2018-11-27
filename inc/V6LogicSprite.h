#pragma once
#include "stdafx.h"
#include "..\\inc\\V6Sprite.h"

namespace VoltaEngine{
	class Sprite;
	struct Animation;
	class LogicSprite{
	public:
		LogicSprite(Sprite* rs) : related_sprite_(rs) { FillEvents(); }
		// 与关联的精灵同步。
		virtual void Sync() = 0;
		void Update(int);
		void StartAnimation(Animation*);

		// 事件注册表。
		map<string, void(*)(int, int, int)> m_events_;
		// 相关联的图像精灵。
		Sprite* related_sprite_;
	protected:
		inline void FillEvents(){

		}
		virtual void UpdateData(int) = 0;
	};
	
	class SimpleLSprite : public LogicSprite{
	public:
		SimpleLSprite(Sprite* rs) : LogicSprite(rs), x_(0.0f), y_(0.0f){}
		SimpleLSprite(Sprite* rs, float _x, float _y) : LogicSprite(rs), x_(_x), y_(_y){}
		void Sync();

		float x_;
		float y_;
	protected:
		void UpdateData(int t){}
	};
}