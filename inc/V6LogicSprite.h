#pragma once
#include "stdafx.h"
#include "..\\inc\\V6Sprite.h"

namespace VoltaEngine{
	class LogicManager;
	class Sprite;
	class Control;

	typedef void(*Event)(Control* sender, int arg1, int arg2, void* exdata);

	class LogicSprite{
	public:
		LogicSprite(Sprite* rs) : related_sprite_(rs){}
		~LogicSprite() { related_sprite_ = nullptr; }
		// ������ľ���ͬ����
		virtual void Sync() = 0;
		void Update(int);
		inline void StartAnimation(Animation a, int lt){
			related_sprite_->StartAnimation(a, lt);
		}
		inline void StartAnimation(TimeFixedAnimation a){
			related_sprite_->StartAnimation(a.ani, a.last_time);
		}

		// �������ͼ���顣
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

	class Control : public SimpleLSprite{
		friend class LogicManager;
	public:
		Control(Sprite* rs) : SimpleLSprite(rs), width_(0), height_(0), enter_(nullptr), leave_(nullptr), highlight_(nullptr),
		enter_ani_(), leave_ani_(), highlight_ani_(){}
		Control(Sprite* rs, float x, float y, int w, int h) : SimpleLSprite(rs, x, y), width_(w), height_(h),
			enter_(nullptr), leave_(nullptr), highlight_(nullptr),
			enter_ani_(), leave_ani_(), highlight_ani_(){}
		~Control(){
			events_.clear();
		}
		inline void RegisterEvent(string ename, Event e){
			events_[ename] = e;
		}
		// ���ž���ʱ�Ķ�����
		inline void DoEnterAni(){ if(enter_ != nullptr) enter_(this); }
		// �����뿪ʱ�Ķ�����
		void DoLeaveAni(){ if(leave_ != nullptr) leave_(this); }
		// ����ǿ��ʱ�Ķ�����
		void DoHighlightAni(){ if(highlight_ != nullptr) highlight_(this); }

		TimeFixedAnimation enter_ani_;
		TimeFixedAnimation leave_ani_;
		TimeFixedAnimation highlight_ani_;
		int width_;
		int height_;
	protected:
		Control(){}

		// �¼�ע���
		map<string, Event> events_;
		void(*enter_)(Control*);
		void(*leave_)(Control*);
		void(*highlight_)(Control*);
	};

	class Button : public Control{
	public:
		Button(Sprite* rs, float x, float y, int w, int h, Event e,
			TimeFixedAnimation* ea = nullptr, TimeFixedAnimation* la = nullptr, TimeFixedAnimation* ha = nullptr);
	private:
		Button(){}
	};

	// ʹ�ü���ѡ��ѡ��Ŀؼ��顣
	class SelectBoard : public Control{

	};

	// ʹ�ü���ѡ��ѡ��Ŀؼ��顣ѡ��ѡ���ָʾ���λ���ǹ̶��ġ�
	class FixedSelectBoard : public Control{

	};
}