#pragma once
#include <Windows.h>
#include <list>
#include <map>
#include <string>

using std::list;
using std::map;
using std::string;
using std::pair;

namespace VoltaEngine{
	#define VKEY	int

	template <typename T> void DisposeList(list<T*>* l){
		auto itor = l->begin();
		while (itor != l->end()){
			delete *itor;
			*itor = nullptr;
			l->erase(itor++);
		}
	}
	template<typename T1, typename T2> void DisposeCOMLib(map<T1, T2*>* l){
		auto itor = l->begin();
		while (itor != l->end()){
			if ((*itor).second != nullptr){
				(*itor).second->Release();
				(*itor).second = nullptr;
			}
			l->erase(itor++);
		}
	}
	template<typename T1, typename T2> void DisposeLib(map<T1, T2*>* l){
		auto itor = l->begin();
		while (itor != l->end()){
			delete (*itor).second;
			(*itor).second = nullptr;
			l->erase(itor++);
		}
	}
	template <typename T1, typename T2> inline void MapInsert(map<T1, T2>* m, T1 key, T2 value){
		m->insert(pair<T1, T2>(key, value));
	}

	// 获得文件名称。
	inline string GetFileName(string file){
		return file.substr(0, file.find("."));
	}
}