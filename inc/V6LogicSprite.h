#pragma once
#include "stdafx.h"
#include "..\\inc\\V6Sprite.h"

namespace VoltaEngine{
	class LogicManager;
	class Control;

	typedef void(*Event)(Control* sender, int arg1, int arg2, void* exdata);

	class LogicSprite{
	public:
		LogicSprite(Sprite* rs) : related_sprite_(rs){}
		~LogicSprite() { related_sprite_ = nullptr; }
		// 与关联的精灵同步。
		virtual void Sync() = 0;
		void Update(int);
		inline void StartAnimation(Animation a, int lt){
			related_sprite_->StartAnimation(a, lt);
		}
		inline void StartAnimation(TimeFixedAnimation a){
			related_sprite_->StartAnimation(a.ani, a.last_time);
		}

		// 相关联的图像精灵。
		Sprite* related_sprite_;
	protected:
		LogicSprite(){}
		virtual void UpdateData(int) = 0;
	};
	
	class SimpleLSprite : public LogicSprite{
	public:
		SimpleLSprite(Sprite* rs) : LogicSprite(rs), x_(0.0f), y_(0.0f){}
		SimpleLSprite(Sprite* rs, float x, float y) : LogicSprite(rs), x_(x), y_(y){}
		void Sync();

		float x_;
		float y_;
	protected:
		SimpleLSprite(){}
		void UpdateData(int t){}
	};

	// 控制器。
	class Control : public SimpleLSprite{
		friend class LogicManager;
	public:
		Control(Sprite* rs) : SimpleLSprite(rs), width_(0), height_(0), enter_(nullptr), leave_(nullptr), highlight_(nullptr),
		enter_ani_(), leave_ani_(), highlight_ani_(){}
		Control(Sprite* rs, float x, float y, float w, float h) : SimpleLSprite(rs, x, y), width_(w), height_(h),
			enter_(nullptr), leave_(nullptr), highlight_(nullptr),
			enter_ani_(), leave_ani_(), highlight_ani_(){}
		~Control(){
			events_.clear();
		}
		inline void RegisterEvent(string ename, Event e){
			events_[ename] = e;
		}
		// 播放经过时的动画。
		inline void DoEnterAni(){ if(enter_ != nullptr) enter_(this); }
		// 播放离开时的动画。
		void DoLeaveAni(){ if(leave_ != nullptr) leave_(this); }
		// 播放强调时的动画。
		void DoHighlightAni(){ if(highlight_ != nullptr) highlight_(this); }

		TimeFixedAnimation enter_ani_;
		TimeFixedAnimation leave_ani_;
		TimeFixedAnimation highlight_ani_;
		float width_;
		float height_;
	protected:
		Control(){}

		// 事件注册表。
		map<string, Event> events_;
		void(*enter_)(Control*);
		void(*leave_)(Control*);
		void(*highlight_)(Control*);
	};

	class Button : public Control{
	public:
		Button(Sprite* rs, float x, float y, float w, float h, Event e,
			TimeFixedAnimation* ea = nullptr, TimeFixedAnimation* la = nullptr, TimeFixedAnimation* ha = nullptr);
	private:
		Button(){}
	};

	// 使用键盘选择选项的控件组。L:Logic的缩写，表示逻辑。
	class LSelectBoard : public Control{
	public:
		// moveup: 光标上移的按键。 idef: 默认光标位置。 ec: 元素总数。
		LSelectBoard(SelectBoard* rs, float x, float y, float sx, float sy, float ew, float eh, int ec, int idef = 0, VKEY moveup = VK_UP, VKEY movedw = VK_DOWN);
		void Sync() override;

		// 当前光标所在的元素。
		int curr_;
		// 元素总数。
		int ecount_;
		int moveup_;
		int movedw_;
	
		float stridex_;
		float stridey_;
		// 每个元素的宽度。
		float ewidth_;
		float eheight_;
	protected:
	};

	// 使用键盘选择选项的控件组。选择选项的指示框的位置是固定的。
	class FixedSelectBoard : public Control{

	};
}