#include "..\\inc\\V6UtilityFuncLib.h"
#include "..\\inc\\V6RenderForm.h"

namespace VoltaEngine{
	bool Init(HINSTANCE hi, HWND hw, int width, int height, bool window){
		VoltaLogicEngine::InitLib();
		return VoltaRenderEngine::Init(hi, hw, (float)width, (float)height, window);
	};

	void VoltaLogicEngine::InitLib(){
		LogicStep *ls = new LogicStep([](int t, list<LogicSprite*>* l, LogicManager* lm){
		 	LONG x = RenderForm::mouse_pos_.x;
			LONG y = RenderForm::mouse_pos_.y;
			auto itor = lm->TopControls.begin();
			while (itor != lm->TopControls.end()){
				Control* c = (static_cast<Control*>(*itor));
				int _x = static_cast<int>(c->x_);
				int _y = static_cast<int>(c->y_);
				if (x >= _x - c->width_ & x <= _x + c->width_ & y >= _y - c->height_ & y <= _y + c->height_){
					if (lm->hover_control_ != c){
						if (lm->hover_control_ != nullptr) lm->hover_control_->DoLeaveAni();
						lm->hover_control_ = c;
						lm->focus_control_ = c;
						c->DoEnterAni();
					}
					return;
				}
				itor++;
			}

			itor = lm->Controls.begin();
			while (itor != lm->Controls.end()){
				Control* c = (static_cast<Control*>(*itor));
				int _x = static_cast<int>(c->x_);
				int _y = static_cast<int>(c->y_);
				if (x >= _x - c->width_ & x <= _x + c->width_ & y >= _y - c->height_ & y <= _y + c->height_){
					if (lm->hover_control_ != c){
						if (lm->hover_control_ != nullptr) lm->hover_control_->DoLeaveAni();
						lm->hover_control_ = c;
						lm->focus_control_ = c;
						c->DoEnterAni();
					}
					return;
				}
				itor++;
			}
			
			if (lm->hover_control_ != nullptr) lm->hover_control_->DoLeaveAni();
			lm->hover_control_ = nullptr;
		});
		LS_lib_["CheckHovering"] = ls;
	}
	void VoltaEngine::Unload(){
		DisposeRecourses();
		VoltaRenderEngine::Dispose();
		VoltaLogicEngine::Dispose();
	}
	void VoltaLogicEngine::Update(int t){
		Manager.Update(t);
		g_time += t;
		VoltaRenderEngine::CBPerFContent_.g_time = XMFLOAT4(static_cast<float>(g_time), 0.0f, 0.0f, 0.0f);
	}
	void LogicManager::Update(int t){
		for_each(Steps.begin(), Steps.end(), [&t, this](LogicStep* step){
			step->Update(t, &Sprites, this);
		});
		for_each(Sprites.begin(), Sprites.end(), [&t, this](LogicSprite* s){
			s->Update(t);
			s->Sync();
		});
		for_each(Controls.begin(), Controls.end(), [&t, this](Control* s){
			s->Update(t);
			s->Sync();
		});
		for_each(TopControls.begin(), TopControls.end(), [&t, this](Control* s){
			s->Update(t);
			s->Sync();
		});
	}	
	void LogicManager::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
		short zdelta;
		RECT rc;
		switch (msg)
		{
		case WM_LBUTTONUP:
			GetWindowRect(hwnd, &rc);
			RaiseClick(LOWORD(lParam) * VoltaRenderEngine::render_width_ / (rc.right - rc.left),
				VoltaRenderEngine::render_height_ - HIWORD(lParam) * VoltaRenderEngine::render_height_ / (rc.bottom - rc.top));
			break;
		case WM_RBUTTONUP:
			GetWindowRect(hwnd, &rc);
			RaiseClickRight(LOWORD(lParam) * VoltaRenderEngine::render_width_ / (rc.right - rc.left),
				VoltaRenderEngine::render_height_ - HIWORD(lParam) * VoltaRenderEngine::render_height_ / (rc.bottom - rc.top));
			break;
		case WM_MOUSEWHEEL:
			zdelta = HIWORD(wParam);
			RaiseWheel(zdelta > 0);
			break;
		case WM_KEYUP:
			RaiseKeyUp(wParam);
			break;
		}
	}

	void LogicManager::AddStep(string s){ Steps.push_back(LS_lib_[s]); }
	void LogicManager::RaiseClick(int x, int y){
		auto itor = TopControls.begin();
		while (itor != TopControls.end()){
			Control* c = (static_cast<Control*>(*itor));
			int _x = static_cast<int>(c->x_);
			int _y = static_cast<int>(c->y_);
			if (x >= _x - c->width_ & x <= _x + c->width_ & y >= _y - c->height_ & y <= _y + c->height_){
				if (c->events_["Click"] != nullptr){
					c->events_["Click"](*itor, x, y, 0);
					focus_control_ = *itor;
				}			
				return;
			}
			itor++;
		}

		itor = Controls.begin();
		while (itor != Controls.end()){
			Control* c = (static_cast<Control*>(*itor));
			int _x = static_cast<int>(c->x_);
			int _y = static_cast<int>(c->y_);
			if (x >= _x - c->width_ & x <= _x + c->width_ & y >= _y - c->height_ & y <= _y + c->height_){
				if (c->events_["Click"] != nullptr){
					c->events_["Click"](*itor, x, y, 0);
					focus_control_ = c;
				}
				return;
			}
			itor++;
		}
	}
	void LogicManager::RaiseClickRight(int x, int y){
		auto itor = TopControls.begin();
		while (itor != TopControls.end()){
			Control* c = (static_cast<Control*>(*itor));
			int _x = static_cast<int>(c->x_);
			int _y = static_cast<int>(c->y_);
			if (x >= _x - c->width_ & x <= _x + c->width_ & y >= _y - c->height_ & y <= _y + c->height_){
				if (c->events_["ClickRight"] != nullptr){
					c->events_["ClickRight"](*itor, x, y, 0);
					focus_control_ = *itor;
				}
				return;
			}
			itor++;
		}

		itor = Controls.begin();
		while (itor != Controls.end()){
			Control* c = (static_cast<Control*>(*itor));
			int _x = static_cast<int>(c->x_);
			int _y = static_cast<int>(c->y_);
			if (x >= _x - c->width_ & x <= _x + c->width_ & y >= _y - c->height_ & y <= _y + c->height_){
				if (c->events_["ClickRight"] != nullptr){
					c->events_["ClickRight"](*itor, x, y, 0);
					focus_control_ = c;
				}
				return;
			}
			itor++;
		}
	}
	void LogicManager::RaiseWheel(bool up){
		if (focus_control_ != nullptr && focus_control_->events_["Wheel"] != nullptr){
			focus_control_->events_["Wheel"](focus_control_, up, 0, 0);
		}
	}
	void LogicManager::RaiseKeyUp(int key){
		if (focus_control_ != nullptr && focus_control_->events_["KeyUp"] != nullptr){
			focus_control_->events_["KeyUp"](focus_control_, key, 0, 0);
		}
	}

	void LogicManager::Clear(){
		Steps.clear();
		DisposeList(&Sprites);
		DisposeList(&Controls);
		DisposeList(&TopControls);
		DisposeLib(&LS_lib_);
	}

	LogicManager VoltaLogicEngine::Manager; 
	long long VoltaLogicEngine::g_time;

	map<string, LogicStep*> LS_lib_;
}