#pragma once
#define LUA_LIB
#include "lua.hpp"
#include <time.h>
#include <stdlib.h>
#include "MersenneTwister.h"

extern MTRand randGen;

/// <summary>
/// Seeds the random number generator.
/// Lua Function -> void seed(int)
/// </summary>
/// <param name="L">Required for lua to call.</param>
/// <
/// <returns></returns>
static int seed(lua_State *L)
{
    /* get number of arguments */
    int n = lua_gettop(L);
    double seedVal = 0;
    int i;

    /* loop through each argument */
    for (i = 1; i <= n; i++)
    {
        /* total the arguments */
        seedVal += lua_tonumber(L, i);
    }

    randGen.seed(seedVal);

    return 0;
}

/// <summary>
/// Returns a random integer
/// Lua Function -> int rand()
/// </summary>
/// <param name="L">Required for lua to call.</param>
/// <returns></returns>
static int rand(lua_State *L)
{
    int randVal = randGen.randInt();

    /* get number of arguments */
    int n = lua_gettop(L);

    //if one was given, limit the generator to numbers less than the arguement
    if(n == 1)
    {
        int limit = lua_tonumber(L, 1);
        randVal = randGen.randInt(limit);
    }

    /* push the random number */
    lua_pushnumber(L, randVal);

    /* return the number of results */
    return 1;
}

static const luaL_Reg diceRandLib[] = 
{
    {"seed", seed},
    {"rand", rand},
    {NULL, NULL}  /* sentinel */
};

int luaopen_DiceRand(lua_State* L);
