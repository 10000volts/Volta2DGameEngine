#include "..\\inc\\V6Input.h"

namespace VoltaEngine{
	// 1��������1���߼�������ֻ�ܽ���1���жϡ�
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