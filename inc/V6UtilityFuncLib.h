#pragma once
#include <list>
#include <map>
#include <string>

using std::list;
using std::map;
using std::string;
using std::pair;

namespace VoltaEngine{
	template <typename T> inline void DisposeList(list<T*>* l){
		auto itor = l->begin();
		while (itor != l->end()){
			delete *itor;
			*itor = nullptr;
			l->erase(itor++);
		}
	};
	template <typename T1, typename T2> inline void MapInsert(map<T1, T2>* m, T1 key, T2 value){
		m->insert(pair<T1, T2>(key, value));
	}
}