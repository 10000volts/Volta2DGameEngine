#pragma once

#include "stdafx.h"
#include "V6RenderForm.h"

namespace VoltaEngine{
	class VoltaInput{
	public:
		// �����������Ϸ���ڵ����꣬�����½�Ϊԭ�㡣
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