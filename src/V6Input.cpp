#include "..\\inc\\V6Input.h"

namespace VoltaEngine{
	// 1个按键在1次逻辑更新中只能进行1次判断。
	bool VoltaInput::GetKeyUp(VKEY key){
		if (GetKeyDown(key)){
			temp_key_states_[key] = true;
			return false;
		}
		if (temp_key_states_[key]){
			temp_key_states_[key] = false;
			return true;
		}
	}

	bool VoltaInput::temp_key_states_[256];
}