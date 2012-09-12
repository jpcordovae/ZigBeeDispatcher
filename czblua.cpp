#include "czblua.h"
#include "CDispatcher.h"

/*
 * constructor of class CZBLua
 *
 * */
CZBLua::CZBLua()
{
}

int CZBLua::getVariablesValues(lua_State *L)
{
    int ret_var_count=0;
    int argc = lua_gettop(L);
    for(int n=1;n<argc;++n)
    {
        std::string variable = lua_tostring(L,n);
        std::cout << variable << std::endl;
    }

    return ret_var_count;
}

int CZBLua::setVariablesValues(lua_State *L)
{
    return 0;
}

