#ifndef COMPAT_H
#define COMPAT_H

#include "lua.hpp"

void socket_luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup);

#endif
