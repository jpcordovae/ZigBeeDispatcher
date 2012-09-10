#ifndef CZBLUA_H
#define CZBLUA_H

#include <lua.hpp>
#include "CDispatcher.h"
#include "lunar.h"

class CZBLua
{
public:

	static const char className[];
    	static Lunar<CZBLua>::RegType methods[];

	CZBLua();
	CZBLua(lua_State *L) {  };
	int getVariablesValues(lua_State *L);
	int setVariablesValues(lua_State *L);
};

const char CZBLua::className[] = "CZBLua";

Lunar<CZBLua>::RegType CZBLua::methods[] = {
	LUNAR_DECLARE_METHOD(CZBLua,getVariablesValues),
	LUNAR_DECLARE_METHOD(CZBLua,setVariablesValues)
};

#endif // CZBLUA_H
