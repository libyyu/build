#ifndef COMPAT_H
#define COMPAT_H

#include "lua.hpp"

#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM < 502
void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup);
#endif

#endif
