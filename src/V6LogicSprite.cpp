#include "..\\inc\\V6LogicSprite.h"

namespace VoltaEngine{
	Button::Button(Sprite* rs, float x, float y, int w, int h, Event e,
		TimeFixedAnimation* ea, TimeFixedAnimation* la, TimeFixedAnimation* ha) : Control(rs, x, y, w, h){
		RegisterEvent("Click", e);
		if (ea != nullptr){
			enter_ani_ = *ea;
			enter_ = [](Control* sender){
				sender->StartAnimation(sender->enter_ani_);
			};
		}
		if (la != nullptr){
			leave_ani_ = *la;
			leave_ = [](Control* sender){
				sender->StartAnimation(sender->leave_ani_);
			};
		}
		if (ha != nullptr){
			highlight_ani_ = *ha;
			highlight_ = [](Control* sender){
				sender->StartAnimation(sender->highlight_ani_);
			};
		}
	}

	void LogicSprite::Update(int t){
		UpdateData(t);

		if (related_sprite_ != nullptr & related_sprite_->animating_){
			related_sprite_->UpdateAnimation(t);
		}
	}

	void SimpleLSprite::Sync(){
		related_sprite_->position_.x = x_;
		related_sprite_->position_.y = y_;
	}
}