#pragma once

#include "stdafx.h"
#include "V6LogicStep.h"
#include "V6LogicSprite.h"

namespace VoltaEngine{
	class VoltaLogicEngine;
	class LogicSprite;
	class LogicStep;
	class Control;

	class LogicManager{
		friend class VoltaLogicEngine;
	public:
		LogicManager() : focus_control_(nullptr), hover_control_(nullptr){}
		void Update(int);
		void Clear();
		inline void AddSprite(LogicSprite* s){ Sprites.push_back(s); }
		inline void AddControl(Control* c){ Controls.push_back(c); }
		inline void AddTopControl(Control* c){ TopControls.push_back(c); }
		inline void AddStep(LogicStep* s){ Steps.push_back(s); }
		void AddStep(string s);
		inline void RemoveSprite(LogicSprite* s){ Sprites.remove(s); delete s; s = nullptr; }
		inline void RemoveControl(Control* c){ Controls.remove(c); delete c; c = nullptr; }
		inline void RemoveTopControl(Control* c){ TopControls.remove(c); delete c; c = nullptr; }
		inline void RemoveStep(LogicStep* s){ Steps.remove(s); delete s; s = nullptr; }
		void Handle(HWND, UINT, WPARAM, LPARAM);

		Control* focus_control_;
		// 鼠标处的控件。可以为nullptr。
		Control* hover_control_;
	protected:
		void RaiseClick(int x, int y);
		void RaiseClickRight(int x, int y);
		void RaiseWheel(bool up);
		void RaiseKeyUp(int key);

		list<LogicSprite*> Sprites;
		list<Control*> Controls;
		list<Control*> TopControls;
		list<LogicStep*> Steps;
	};
	class VoltaLogicEngine{
	public:
		// 初始化常用逻辑库。
		static void InitLib();
		static void Update(int t);
		inline static void Dispose(){
			Manager.Clear();
		}
		inline static void Handle(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){ Manager.Handle(hwnd, message, wParam, lParam); }

		// 其实应该作封装。
		static LogicManager Manager;
		// 全局时间。
		static long long g_time;
	};
}