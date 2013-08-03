#pragma once
#define LUA_LIB
#include "lua.hpp"
#include <time.h>
#include <stdlib.h>

static int average(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* loop through each argument */
	for (i = 1; i <= n; i++)
	{
		/* total the arguments */
		sum += lua_tonumber(L, i);
	}

	/* push the average */
	lua_pushnumber(L, sum / n);

	/* push the sum */
	lua_pushnumber(L, sum);

	/* return the number of results */
	return 2;
}

static int rand(lua_State *L)
{
    srand(clock());

    int randVal = rand();

	/* push the random number */
	lua_pushnumber(L, randVal);

	/* return the number of results */
	return 1;
}

static const luaL_Reg diceRandLib[] = 
{
    {"average", average},
    {"rand", rand},
    {NULL, NULL}  /* sentinel */
};

int luaopen_DiceRand(lua_State* L);
