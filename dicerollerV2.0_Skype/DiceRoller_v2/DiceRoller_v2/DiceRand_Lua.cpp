#include "stdafx.h"
#include "DiceRand_Lua.h"

MTRand randGen;

int luaopen_DiceRand(lua_State* L)
{
    luaL_newlib( L, diceRandLib );
    return 1;
}