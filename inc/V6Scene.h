#pragma once

#include "V6RenderEngine.h"
namespace VoltaEngine{
	class Scene{
	public:
		// ����������ó���ʱ�ľ�����Ϊ��
		Scene(void(*func)()) : toscene(func){}

		void ToScene(){
			VoltaRenderEngine::Manager.Clear();
			toscene();
		}
	protected:
		void (*toscene)();
	};

}