#include "..\\inc\\V6LogicSprite.h"

namespace VoltaEngine{
	Button::Button(Sprite* rs, float x, float y, float w, float h, Event e,
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

	LSelectBoard::LSelectBoard(SelectBoard* rs, float x, float y, float sx, float sy, float ew, float eh, int ec, int idef, VKEY moveup, VKEY movedw) :
		Control(rs, x, y, 0.0f, 0.0f), stridex_(sx), stridey_(sy), ewidth_(ew), eheight_(eh), ecount_(ec), curr_(idef), moveup_(moveup), movedw_(movedw){
		RegisterEvent("KeyUp", [](Control* sender, int arg1, int arg2, void* exdata){
			LSelectBoard* lsb = (LSelectBoard*)sender;
			if (lsb->moveup_ == arg1){
				if (lsb->curr_){
					lsb->curr_--;
				}
				else{
					lsb->curr_ = lsb->ecount_ - 1;
				}
			}
			else if (lsb->movedw_ == arg1){
				if (lsb->curr_ < lsb->ecount_ - 1){
					lsb->curr_++;
				}
				else{
					lsb->curr_ = 0;
				}
			}
		});
	}
	void LSelectBoard::Sync(){
		SelectBoard* sb = (SelectBoard*)related_sprite_;
		for (int i = 0; i < ecount_; ++i){
			sb->children_[i]->position_.x = x_ - stridex_ * static_cast<float>(i);
			sb->children_[i]->position_.y = y_ - stridey_ * static_cast<float>(i);
		}
		sb->border_->position_.x = x_ - stridex_ * static_cast<float>(curr_);
		sb->border_->position_.y = y_ - stridey_ * static_cast<float>(curr_);
	}
}