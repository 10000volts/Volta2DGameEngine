#include "..\\inc\\V6LogicSprite.h"

namespace VoltaEngine{
	void LogicSprite::Update(int t){
		UpdateData(t);

		if (related_sprite_ != nullptr & related_sprite_->animating_){
			related_sprite_->UpdateAnimation(t);
		}
	}
	void LogicSprite::StartAnimation(Animation* a){
		related_sprite_->StartAnimation(a);
	}

	void SimpleLSprite::Sync(){
		related_sprite_->position_.x = x_;
		related_sprite_->position_.y = y_;
	}
}