#pragma once

#include "stdafx.h"
#include "V6RenderForm.h"

namespace VoltaEngine{
	class VoltaInput{
	public:
		// 获得鼠标相对游戏窗口的坐标，以左下角为原点。
		inline static void GetMousePos(int& x, int& y){
			x = RenderForm::mouse_pos_.x;
			y = RenderForm::mouse_pos_.y;
		}

		inline static bool GetKeyDown(VKEY key){ return ::GetAsyncKeyState(key) & 0x8000;}
	    static bool GetKeyUp(VKEY key);
	protected:
		static bool temp_key_states_[256];
	};
}