#pragma once

#include "V6RenderEngine.h"
namespace VoltaEngine{
	class Scene{
	public:
		// 参数：进入该场景时的具体行为。
		Scene(void(*func)()) : toscene(func){}

		void ToScene(){
			VoltaRenderEngine::Manager.Clear();
			toscene();
		}
	protected:
		void (*toscene)();
	};

}